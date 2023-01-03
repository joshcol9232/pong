cmake_minimum_required(VERSION 3.5)

project(pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories("")

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

add_executable(pong main.cpp
               constants.h
               ball.h ball.cpp
               paddle.h paddle.cpp)

target_link_libraries(pong PRIVATE Eigen3::Eigen sfml-audio sfml-graphics sfml-network sfml-system)

#install(TARGETS pong
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

