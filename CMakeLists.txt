cmake_minimum_required(VERSION 3.17)
project(talkserver)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32)

add_executable(talkserver main.cpp store.cpp help.cpp Client.h)