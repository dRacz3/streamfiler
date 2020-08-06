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
#include <algorithm>
#include "Connection.h"
#include <vector>

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

        Connection welcomeSocketConnection(connection_id++, m_welcomeSocket);
        m_logger.info("Welcome socket fd:" + std::to_string(m_welcomeSocket));
        m_connections.push_back(welcomeSocketConnection);

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
            m_logger.info("Background thread is running!");
            while (m_running) {
                std::for_each(m_connections.begin(), m_connections.end(), [&](Connection &c) {
                    processConnection(c);
                });

                cleanupConnections();
                usleep(500);
            }
        });
        m_backgroundThread.detach();
    }

    void handleNewConnection() {
        int newFileDescriptor = -1;
        do {
            newFileDescriptor = accept(m_welcomeSocket, nullptr, nullptr);
            if (newFileDescriptor < 0) {
                if (errno != EWOULDBLOCK) {
                    m_logger.error("Failed to accept connection");
                }
                return;
            }
            int activeConnectionCount = getNumberOfActiveConnections();
            if (activeConnectionCount < MAXIMUM_CONNECTIONS + 1) {
                Connection newConnection(connection_id++, newFileDescriptor);
                m_connections.push_back(newConnection);
                m_logger.info("New incoming connection " + newConnection.toString());
            } else {
                m_logger.warning("Failed to accept connection: maximum number of m_connections reached");
                close(newFileDescriptor);
            }
        } while (newFileDescriptor != -1);
    }

    void processConnection(Connection &c) {
        if (poll(c.data, 1, 50) < 0) {
            m_logger.error("poll() failed");
        }

        if (c.data[0].revents == 0) {
            return;
        }

        if (c.data[0].revents & POLLHUP) {
            m_logger.info("Connection " + std::to_string(c.data[0].fd) + " closed: hung up");
            c.close();
            return;
        }

        if (c.data[0].revents != POLLIN) {
            m_logger.info(
                    "Connection :" + c.toString() + " closed, revents: " + std::to_string(c.data[0].revents));
            c.close();
            return;
        }

        if (c.data[0].fd == m_welcomeSocket) {
            m_logger.debug("Checking welcome socket.");
            handleNewConnection();
        } else {
            char buffer[80];
            memset(buffer, 0, sizeof(buffer));
            int recvrc = recv(c.data[0].fd, buffer, sizeof(buffer), 0);
            if (recvrc < 0) {
                if (errno != EWOULDBLOCK) {
                    m_logger.info("Connection closed: " + c.toString() + " with errno: " + std::strerror(errno));
                    c.close();
                }
                return;
            }
            if (recvrc == 0) {
                m_logger.info("Connection closed, recv == 0: " + c.toString() + " with errno: " + std::strerror(errno));
                c.close();
                return;
            }

            std::stringstream ss;
            ss << "ECHO:" << buffer;
            ss << "\n";
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, ss.str().c_str());
            if (send(c.data[0].fd, buffer, strlen(buffer), 0) < 0) {
                c.close();
                return;
            }
        }
    }

    int getNumberOfActiveConnections() {
        return std::count_if(m_connections.begin(), m_connections.end(), [](Connection c) { return !c.isClosed(); });
    }

    void cleanupConnections() {
        m_connections.erase(std::remove_if(m_connections.begin(), m_connections.end(), [&](Connection c) {
            m_logger.debug("Checking to remove:" + c.toString());
            return c.isClosed();
        }), m_connections.end());
    }

    int connection_id = 0;
    std::thread m_backgroundThread;
    bool m_running{false};
    Logger m_logger;
    int m_welcomeSocket{};
    std::vector<Connection> m_connections;

};

#endif //STREAMFILER_MULTICLIENTTCPSTREAMLISTENER_H
