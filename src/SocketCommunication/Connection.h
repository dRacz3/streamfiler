#ifndef STREAMFILER_CONNECTION_H
#define STREAMFILER_CONNECTION_H


#include <poll.h>

struct Connection {
    void close();

    [[nodiscard]] bool isClosed() const;

    struct pollfd data[1];

};


#endif //STREAMFILER_CONNECTION_H
