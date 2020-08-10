cmake_minimum_required(VERSION 3.17)
project(streamfiler)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include_directories(src
        src/DiskWriter
        src/SocketCommunication
        src/Utilities
        third-party/Tokenbucket)

add_executable(streamfiler
        Server/main.cpp
        src/DiskWriter/ConsoleWriter.cpp
        src/DiskWriter/DiskWriter.cpp
        src/SocketCommunication/MultiClientTCPStreamListener.cpp
        src/SocketCommunication/Connection.cpp
        )

target_link_libraries(streamfiler PRIVATE Threads::Threads stdc++fs)


add_executable(streamfilerclient Client/main.cpp)
