#ifndef STREAMFILER_CONNECTION_H
#define STREAMFILER_CONNECTION_H


#include <poll.h>
#include <string>

struct Connection {
    void close();

    [[nodiscard]] bool isClosed() const;

    struct pollfd data[1];

    std::string toString();

};


#endif //STREAMFILER_CONNECTION_H
