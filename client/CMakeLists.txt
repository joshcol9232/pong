cmake_minimum_required(VERSION 3.5)

project(pong_client LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows build (uncomment)
# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/i686-w64-mingw32
    /home/josh/mingw-install)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# --- end ---

#include_directories("")

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

add_executable(pong_client main.cpp
               constants.h
               msg_type.h msg_type.cpp)

target_link_libraries(pong_client PRIVATE Eigen3::Eigen sfml-audio sfml-graphics sfml-network sfml-system)

