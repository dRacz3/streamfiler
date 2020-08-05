#ifndef STREAMFILER_TCPSTREAMREADER_H
#define STREAMFILER_TCPSTREAMREADER_H


#include "FileWriter.h"

class TCPStreamReader {
public:
    TCPStreamReader(int connections, int limit, float timeout, FileWriter writer);

    void listen(int maxConnectionNumber);

    void shutdown();

protected:
    int m_maxConnectionCount;
    int m_dataLimit;
    float m_timeout;
    FileWriter m_writer;
};


#endif //STREAMFILER_TCPSTREAMREADER_H
