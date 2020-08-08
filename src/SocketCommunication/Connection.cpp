#include "Connection.h"

#include <Utilities/Logger.h>
#include <sys/poll.h>
#include <sys/socket.h>

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
   return "Connection ID: " + std::to_string(connection_id) + " [fd: " + std::to_string(data[0].fd) + "]";
}

Connection::Connection(int id, int fd, std::shared_ptr<ITextOutput> streamOutput)
    : connection_id(id), m_logger("con-" + std::to_string(id)), m_output(std::move(streamOutput))
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
void Connection::processMessage()
{
   char buffer[80];
   memset(buffer, 0, sizeof(buffer));
   int recvrc = recv(data[0].fd, buffer, sizeof(buffer), 0);
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

   ss << "ECHO:" << buffer;
   ss << "\n";
   memset(buffer, 0, sizeof(buffer));
   strcpy(buffer, ss.str().c_str());
   if (send(data[0].fd, buffer, strlen(buffer), 0) < 0)
   {
      close();
      return;
   }
}
