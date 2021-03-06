#include "Connection.h"

#include <Utilities/Logger.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

void Connection::close()
{
   m_isClosed = true;
   data[0].fd = -1;
   data[0].revents = 0;
   m_logger.info("Closing " + toString());
   m_output->close();
}

bool Connection::isClosed() const
{
   m_logger.debug(toString() + "checked in " + __func__);
   return m_isClosed;
}

std::string Connection::toString() const
{
   std::stringstream ss;
   ss << "Connection ID : " << std::to_string(connection_id);
   ss << " | Timeout Active: " << m_connectionProperties.timeoutActive
      << ", value: " << m_connectionProperties.m_timeoutDelay.count() << "ms";
   return ss.str();
}

Connection::Connection(int id, int fd, ConnectionProperties timeout, std::shared_ptr<ITextOutput> streamOutput)
    : connection_id(id),
      m_logger("con-" + std::to_string(id)),
      m_output(std::move(streamOutput)),
      m_connectionProperties(timeout)
{
   data[0].fd = fd;
   data[0].events = POLLIN;
}
bool Connection::update()
{
   if (poll(data, 1, 50) < 0)
   {
      m_logger.error("poll() failed");
      return false;
   }

   if (data[0].revents == 0)
   {
      return false;
   }

   m_lastMessageTimestamp = std::chrono::steady_clock::now();
   return true;
}
bool Connection::checkConnection()
{
   if (data[0].revents & POLLHUP)
   {
      m_logger.info("Connection " + std::to_string(data[0].fd) + " closed: hung up");
      return false;
   }

   if (data[0].revents != POLLIN)
   {
      m_logger.info("Connection :" + toString() + " closed, revents: " + std::to_string(data[0].revents));
      return false;
   }

   return true;
}
void Connection::processMessage(TokenBucket& tokens)
{
   int BUFFER_SIZE = m_connectionProperties.bandwidthLimit / 2;
   while (!tokens.consume(BUFFER_SIZE))
   {
      m_logger.debug("Waiting for tokens...");
      usleep(500);
   }
   char buffer[BUFFER_SIZE];
   memset(buffer, 0, sizeof(buffer));
   int recvrc = read(data[0].fd, buffer, sizeof(buffer));
   m_logger.debug("Received: " + std::string(buffer));
   if (recvrc < 0)
   {
      if (errno != EWOULDBLOCK)
      {
         m_logger.info("Connection closed: " + toString() + " with errno: " + std::strerror(errno));
         close();
      }
      return;
   }
   if (recvrc == 0)
   {
      m_logger.info("Connection closed, recv == 0: " + toString() + " with errno: " + std::strerror(errno));
      close();
      return;
   }

   std::stringstream ss;
   if (m_output)
   {
      m_output->write(buffer);
   }

   ss << "OK\n";
   memset(buffer, 0, sizeof(buffer));
   strcpy(buffer, ss.str().c_str());
   m_lastMessageTimestamp = std::chrono::steady_clock::now();
   if (send(data[0].fd, buffer, strlen(buffer), 0) < 0)
   {
      close();
      return;
   }
}
int Connection::getFileDescriptor() const { return data[0].fd; }

bool Connection::isTimedOut()
{
   if (!m_connectionProperties.timeoutActive)
   {
      return false;
   }
   auto now = std::chrono::steady_clock::now();

   if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastMessageTimestamp).count()
       > m_connectionProperties.m_timeoutDelay.count())
   {
      m_logger.info("Has not received data over the connection for a while now. Connection timed out");
      return true;
   }
   return false;
}
