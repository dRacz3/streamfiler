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
   //! Flag to enable/disable timeout.
   bool timeoutActive;
   //! Desired timeout in milliseconds.
   std::chrono::duration<float, std::milli> m_timeoutDelay;
};

class Connection
{
  public:
   /**
    * Create connection class
    * @param id - Connection id
    * @param fd - Retrieved file descriptor
    * @param timeout - Timeout parameters
    * @param streamOutput - Target for the received stream to be redirected.
    */
   Connection(int id, int fd, TimeoutProperties timeout, std::shared_ptr<ITextOutput> streamOutput);

   /**
    * Close the connection
    */
   void close();

   /**
    * Check for new event on the connection.
    * @return true if any event was received, false otherwise.
    */
   bool update();

   /**
    * Perform health check on connection.
    * @return true, if connection is alright, false otherwise.
    */
   bool checkConnection();

   /**
    * Process the message.
    * @param tokens - TokenBucket object to handle bandwidth limitation.
    */
   void processMessage(TokenBucket& tokens);

   /**
    * Check for the connection state.
    * @return true if the connection has been closed.
    */
   [[nodiscard]] bool isClosed() const;

   /**
    * @return String representation of the connection-
    */
   [[nodiscard]] std::string toString() const;

   /**
    * @return the associated file descriptor.
    */
   [[nodiscard]] int getFileDescriptor() const;

   /**
    * Check if the connection has timed out.
    * @return true if timed out, false otherwise.
    */
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
