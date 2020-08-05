cmake_minimum_required(VERSION 3.17)
project(streamfiler)

set(CMAKE_CXX_STANDARD 17)



add_executable(streamfiler src/main.cpp src/FileWriter.cpp src/FileWriter.h src/TCPStreamReader.cpp src/TCPStreamReader.h)