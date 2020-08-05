#ifndef STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
#define STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H

#include <sys/poll.h>
#include <mutex>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <sstream>
#include <unistd.h>
#include <fcntl.h>

#include <cerrno>
#include <thread>
#include <Utilities/Logger.h>
#include <map>
#include "Connection.h"

template<size_t MAXIMUM_CONNECTIONS>
class MultiClientTCPStreamListener {
public:
    MultiClientTCPStreamListener() : m_logger("TCPListener") {
    };

    virtual ~MultiClientTCPStreamListener() { stop(); };

    bool init(int port) {
        m_logger.info("Initializing on port:" + std::to_string(port));
        struct sockaddr_in serverAddr{};
        m_welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
        if (m_welcomeSocket < 0) {
            m_logger.error("Error creating socket.");
            return false;
        }


        int yes = 1;
        setsockopt(m_welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (ioctl(m_welcomeSocket, FIONBIO, (char *) &yes) < 0) {
            m_logger.error("Error setting the socket non-blocking.");
            return false;
        }

        int flags = fcntl(m_welcomeSocket, F_GETFL, 0);
        fcntl(m_welcomeSocket, F_SETFL, flags | O_NONBLOCK);

        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(m_welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
            m_logger.error("Error binding socket.");
            return false;
        }

        if (listen(m_welcomeSocket, MAXIMUM_CONNECTIONS) == 0) {
            m_logger.info("Listening");
        } else {
            m_logger.error("Error in setting the socket to listen mode");
            return false;
        }

        connections[0].data.fd = m_welcomeSocket;
        connections[0].data.events = POLLIN;
        startBackgroundThread();
        return true;
    }

    bool stop() {
        m_logger.info("Stopping listening..");
        m_running = false;
        if (m_backgroundThread.joinable()) {
            m_logger.info("Waiting for background thread.");
            m_backgroundThread.join();
        } else {
            m_logger.info("Background thread is not running...");


        }
        return false;
    }

protected:
    void startBackgroundThread() {
        m_logger.info("Starting internal thread");
        m_running = true;
        m_backgroundThread = std::thread([&]() {

        });
        m_backgroundThread.detach();
    }

    std::thread m_backgroundThread;
    bool m_running{false};
    Logger m_logger;
    int m_welcomeSocket{};
    std::array<Connection, MAXIMUM_CONNECTIONS> connections;
};

#endif //STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
