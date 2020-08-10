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
      int port;
      size_t maxConnectionCount;
   };

   explicit MultiClientTCPStreamListener(Parameters params);

   virtual ~MultiClientTCPStreamListener();

   bool init();
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
