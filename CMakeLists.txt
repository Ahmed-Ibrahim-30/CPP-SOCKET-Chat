cmake_minimum_required(VERSION 3.26)
project(ClientTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(ClientTest
        Client.cpp)
target_link_libraries(ClientTest ws2_32)

add_executable(ServerTest
        Server.cpp)
target_link_libraries(ServerTest ws2_32)
