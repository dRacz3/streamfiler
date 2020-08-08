#include "Connection.h"

void Connection::close() {
    m_isClosed = true;

    m_logger.info("Closing " + toString());
}

bool Connection::isClosed() const {
    m_logger.debug(toString() + "checked in " + __func__);
    return m_isClosed;
}

std::string Connection::toString() const {
    return "Connection ID: " + std::to_string(connection_id) + " [fd: " + std::to_string(data[0].fd) +
           "]";
}

Connection::Connection(int id, int fd) : connection_id(id), m_logger("Connection-" + std::to_string(id)) {
    data[0].fd = fd;
    data[0].events = POLLIN;
}
