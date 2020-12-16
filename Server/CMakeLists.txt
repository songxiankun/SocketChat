cmake_minimum_required(VERSION 3.17)
project(Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(server server.cpp server.hpp basesocket.cpp basesocket.hpp)