cmake_minimum_required(VERSION 3.17)
project(Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(server Config.hpp
        Socket.hpp Socket.cpp
        RecordLogs.hpp RecordLogs.cpp
        BaseLogs.hpp Test.cpp)