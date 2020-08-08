#ifndef STREAMFILER_CONNECTION_H
#define STREAMFILER_CONNECTION_H

#include <Utilities/Logger.h>
#include <poll.h>

#include <string>

struct Connection
{
   Connection(int id, int fd);

   void close();

   bool update();

   bool checkConnection();

   void processMessage();

   [[nodiscard]] bool isClosed() const;

   [[nodiscard]] std::string toString() const;

   int connection_id;
   bool m_isClosed = false;
   struct pollfd data[1]{};
   Logger m_logger;
};

#endif  // STREAMFILER_CONNECTION_H
