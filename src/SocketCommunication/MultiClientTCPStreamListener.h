#ifndef STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
#define STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H

#include <sys/poll.h>
#include <Utilities/Logger.h>

template<int MAXIMUM_CONNECTIONS>
class MultiClientTCPStreamListener {
public:
    MultiClientTCPStreamListener() : m_logger("TCPListener") {};

    virtual ~MultiClientTCPStreamListener() { stop(); };

    bool init(int port) {
        m_logger.info("Initializing on port:" + std::to_string(port));
        return false;
    }

    bool stop() {
        m_logger.info("Stopping listening..");
        return false;
    }

protected:
    void startInternalThread() {
        m_logger.info("Starting internal thread");
    }

    Logger m_logger;
    struct pollfd m_sConnectedClients[MAXIMUM_CONNECTIONS]{};
};

#endif //STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
