#ifndef STREAMFILER_CONNECTION_H
#define STREAMFILER_CONNECTION_H

#include <DiskWriter/ITextOutput.h>
#include <Utilities/Logger.h>
#include <poll.h>

#include <memory>
#include <string>
#include "Tokenbucket.h"

struct Connection
{
   Connection(int id, int fd, std::shared_ptr<ITextOutput> streamOutput);
   void close();

   bool update();

   bool checkConnection();

   void processMessage(TokenBucket tokens);

   [[nodiscard]] bool isClosed() const;

   [[nodiscard]] std::string toString() const;

   int connection_id;
   bool m_isClosed = false;
   struct pollfd data[1]{};
   Logger m_logger;

   std::shared_ptr<ITextOutput> m_output;
};

#endif  // STREAMFILER_CONNECTION_H
