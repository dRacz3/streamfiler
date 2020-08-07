#include "Connection.h"

void Connection::close() {
    data.fd = -1;
    data.revents = 0;
}

bool Connection::isClosed() const {
    return data.fd == -1 && data.revents == 0;
}
