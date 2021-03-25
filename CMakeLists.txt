cmake_minimum_required(VERSION 3.16)
project(livepush)

set(CMAKE_CXX_STANDARD 20)

add_executable(livepush main.cpp
        fileread/jsonfileread.cpp server/server.cpp server/server.h)

target_link_libraries(livepush curl jsoncpp)