#include "TCPStreamReader.h"

#include <utility>
#include "ConsoleWriter.h"
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>

TCPStreamReader::TCPStreamReader(int connections, int limit, float timeout, ConsoleWriter writer) :
        m_maxConnectionCount(connections), m_dataLimit(limit), m_timeout(timeout), m_writer(std::move((writer))) {

}

bool TCPStreamReader::startListening(int port) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        return false;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return false;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        return false;
    }
    if (listen(server_fd, m_maxConnectionCount) < 0)
    {
        perror("startListening");
        return false;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        return false;
    }
    valread = read( new_socket , buffer, m_dataLimit);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return true;
}

void TCPStreamReader::shutdown() {
    m_running = false;
}
