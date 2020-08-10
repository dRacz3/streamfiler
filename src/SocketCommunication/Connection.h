#ifndef STREAMFILER_CONNECTION_H
#define STREAMFILER_CONNECTION_H

#include <DiskWriter/ITextOutput.h>
#include <Utilities/Logger.h>
#include <poll.h>

#include <memory>
#include <string>

#include "Tokenbucket.h"

struct TimeoutProperties
{
   bool timeoutActive;
   std::chrono::duration<float, std::milli> m_timeoutDelay;
};

class Connection
{
  public:
   Connection(int id, int fd, TimeoutProperties timeout, std::shared_ptr<ITextOutput> streamOutput);
   void close();

   bool update();

   bool checkConnection();

   void processMessage(TokenBucket tokens);

   [[nodiscard]] bool isClosed() const;

   [[nodiscard]] std::string toString() const;

   [[nodiscard]] int getFileDescriptor() const;

   bool isTimedOut();
  protected:
   int connection_id;
   bool m_isClosed = false;

   struct pollfd data[1]{};
   Logger m_logger;

   std::shared_ptr<ITextOutput> m_output;

   std::chrono::steady_clock::time_point m_lastMessageTimestamp;
   TimeoutProperties m_timeoutProperties;
};

#endif  // STREAMFILER_CONNECTION_H
