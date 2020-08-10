#ifndef STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
#define STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H

#include <Utilities/Logger.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "Connection.h"
#include "Tokenbucket.h"

class MultiClientTCPStreamListener
{
  public:
   struct Parameters
   {
      //! Connection port
      int port;
      //! Maximum number of connections.
      size_t maxConnectionCount;
      //! Timeout value in milliseconds.
      int timeout;
      //! Location for the output stream.
      std::string folder;
      //! Bandwidth limitation [kilobyte/s]
      int bandwithLimit;
   };

   /**
    * Create a MultiClientTCPStreamListener
    * @param params - associated parameter list.
    */
   explicit MultiClientTCPStreamListener(Parameters params);

   virtual ~MultiClientTCPStreamListener();

   /**
    * Initialize the listener, start background thread.
    * @return true on success, false otherwise.
    */
   bool init();

   /**
    * Gracefully stop the listener thread.
    * @return true on success, false otherwise.
    */
   bool stop();

  protected:
   void startBackgroundThread();
   void handleNewConnection();
   void processConnection(Connection& c);
   int getNumberOfActiveConnections();
   void cleanupConnections();

   int connection_id = 0;
   Parameters m_params;
   std::thread m_backgroundThread;
   bool m_running{false};
   Logger m_logger;
   int m_welcomeSocket{};
   std::vector<Connection> m_connections;

   TokenBucket m_tokenBucket;
};

#endif  // STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
