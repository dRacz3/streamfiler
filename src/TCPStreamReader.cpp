#include "TCPStreamReader.h"

#include <utility>
#include "FileWriter.h"


TCPStreamReader::TCPStreamReader(int connections, int limit, float timeout, FileWriter writer) :
        m_maxConnectionCount(connections), m_dataLimit(limit), m_timeout(timeout), m_writer(std::move((writer))) {

}

void TCPStreamReader::listen(int maxConnectionNumber) {

}

void TCPStreamReader::shutdown() {

}
