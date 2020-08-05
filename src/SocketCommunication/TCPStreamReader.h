#ifndef STREAMFILER_TCPSTREAMREADER_H
#define STREAMFILER_TCPSTREAMREADER_H


#include "ConsoleWriter.h"

class TCPStreamReader {
public:
    TCPStreamReader(int connections, int limit, float timeout, ConsoleWriter writer);

    bool startListening(int port);

    void shutdown();

protected:
    int m_maxConnectionCount;
    int m_dataLimit;
    float m_timeout;
    ConsoleWriter m_writer;
    bool m_running = false;
};


#endif //STREAMFILER_TCPSTREAMREADER_H
