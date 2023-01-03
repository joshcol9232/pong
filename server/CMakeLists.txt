cmake_minimum_required(VERSION 3.5)

project(pong_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories("")

find_package(SFML 2.5.1 COMPONENTS system network REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

add_executable(pong_server main.cpp
               constants.h
               ball.h ball.cpp
               paddle.h paddle.cpp
               msg_type.h msg_type.cpp)

target_link_libraries(pong_server PRIVATE Eigen3::Eigen sfml-network sfml-system)

