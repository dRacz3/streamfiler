cmake_minimum_required(VERSION 3.17)
project(streamfiler)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include_directories(src
        src/DiskWriter
        src/SocketCommunication
        src/Utilities)

add_executable(streamfiler
        Server/main.cpp
        src/DiskWriter/ConsoleWriter.cpp
        src/SocketCommunication/MultiClientTCPStreamListener.h
        src/SocketCommunication/Connection.cpp
        )

target_link_libraries(streamfiler PRIVATE Threads::Threads)


add_executable(streamfilerclient Client/main.cpp)
