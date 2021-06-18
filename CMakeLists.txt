cmake_minimum_required(VERSION 3.7)
project(lydia CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(narwhal)
add_subdirectory(binproto)
add_subdirectory(protocol)
add_subdirectory(server)
