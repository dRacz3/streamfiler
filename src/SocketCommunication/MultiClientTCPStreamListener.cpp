#include "MultiClientTCPStreamListener.h"

#include <DiskWriter/ConsoleWriter.h>
#include <DiskWriter/DiskWriter.h>

#include <utility>

MultiClientTCPStreamListener::~MultiClientTCPStreamListener() { stop(); }
MultiClientTCPStreamListener::MultiClientTCPStreamListener(MultiClientTCPStreamListener::Parameters params)
    : m_params(std::move(params)), m_logger("TCPListener"), m_tokenBucket(26, 26)
{}

bool MultiClientTCPStreamListener::init()
{
   m_logger.info("Initializing on port:" + std::to_string(m_params.port));
   struct sockaddr_in serverAddr
   {
   };
   m_welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
   if (m_welcomeSocket < 0)
   {
      m_logger.error("Error creating socket.");
      return false;
   }

   int yes = 1;
   setsockopt(m_welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

   if (ioctl(m_welcomeSocket, FIONBIO, (char*)&yes) < 0)
   {
      m_logger.error("Error setting the socket non-blocking.");
      return false;
   }

   int flags = fcntl(m_welcomeSocket, F_GETFL, 0);
   fcntl(m_welcomeSocket, F_SETFL, flags | O_NONBLOCK);

   memset(&serverAddr, 0, sizeof(serverAddr));

   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(m_params.port);
   serverAddr.sin_addr.s_addr = INADDR_ANY;

   if (bind(m_welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
   {
      m_logger.error("Error binding socket.");
      return false;
   }

   if (listen(m_welcomeSocket, m_params.maxConnectionCount) == 0)
   {
      m_logger.info("Listening");
   }
   else
   {
      m_logger.error("Error in setting the socket to listen mode");
      return false;
   }

   Connection welcomeSocketConnection(connection_id++,
                                      m_welcomeSocket,
                                      {false, std::chrono::duration<float, std::milli>(m_params.timeout)},
                                      std::make_shared<ConsoleWriter>("WelcomeSocket"));
   m_logger.info("Welcome socket fd:" + std::to_string(m_welcomeSocket));
   m_connections.push_back(welcomeSocketConnection);

   startBackgroundThread();
   return true;
}

bool MultiClientTCPStreamListener::stop()
{
   m_logger.info("Stopping listening..");
   m_running = false;
   if (m_backgroundThread.joinable())
   {
      m_logger.info("Waiting for background thread.");
      m_backgroundThread.join();
   }
   else
   {
      m_logger.info("Background thread is not running...");
   }
   return false;
}

void MultiClientTCPStreamListener::startBackgroundThread()
{
   m_logger.info("Starting internal thread");
   m_running = true;
   m_backgroundThread = std::thread([&]() {
      m_logger.info("Background thread is running!");
      while (m_running)
      {
         std::for_each(m_connections.begin(), m_connections.end(), [&](Connection& c) { processConnection(c); });

         cleanupConnections();
         usleep(500);
      }
   });
   m_backgroundThread.detach();
}

void MultiClientTCPStreamListener::handleNewConnection()
{
   int newFileDescriptor = -1;
   do
   {
      newFileDescriptor = accept(m_welcomeSocket, nullptr, nullptr);
      if (newFileDescriptor < 0)
      {
         if (errno != EWOULDBLOCK)
         {
            m_logger.error("Failed to accept connection");
         }
         return;
      }
      int activeConnectionCount = getNumberOfActiveConnections();
      if (activeConnectionCount < m_params.maxConnectionCount + 1)
      {
         connection_id++;
         Connection newConnection(
             connection_id,
             newFileDescriptor,
             {m_params.timeout != -1, std::chrono::duration<float, std::milli>(m_params.timeout)},
             // TODO: Pass current timestamp for filename
             std::make_shared<DiskWriter>(m_params.folder, "Connection" + std::to_string(connection_id)));
         m_connections.push_back(newConnection);
         m_logger.info("New incoming connection " + newConnection.toString());
      }
      else
      {
         m_logger.warning("Failed to accept connection: maximum number of m_connections reached");
         close(newFileDescriptor);
      }
   } while (newFileDescriptor != -1);
}

void MultiClientTCPStreamListener::processConnection(Connection& c)
{
   bool anyUpdate = c.update();

   if (!anyUpdate)
   {
      if (c.isTimedOut())
      {
         c.close();
      }
      return;
   }

   if (!c.checkConnection())
   {
      c.close();
      return;
   }

   if (c.getFileDescriptor() == m_welcomeSocket)
   {
      m_logger.debug("Checking welcome socket.");
      handleNewConnection();
   }
   else
   {
      c.processMessage(m_tokenBucket);
   }
}

int MultiClientTCPStreamListener::getNumberOfActiveConnections()
{
   return std::count_if(m_connections.begin(), m_connections.end(), [](const Connection& c) { return !c.isClosed(); });
}

void MultiClientTCPStreamListener::cleanupConnections()
{
   m_connections.erase(std::remove_if(m_connections.begin(),
                                      m_connections.end(),
                                      [&](const Connection& c) {
                                         m_logger.debug("Checking to remove:" + c.toString());
                                         return c.isClosed();
                                      }),
                       m_connections.end());
}
