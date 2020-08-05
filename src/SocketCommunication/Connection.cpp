#include "Connection.h"

void Connection::close() {
    data[0].fd = -1;
    data[0].revents = 0;
}

bool Connection::isClosed() const {
    return data[0].fd == -1 && data[0].revents == 0;
}
