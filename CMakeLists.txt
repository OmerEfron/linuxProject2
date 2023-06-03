cmake_minimum_required(VERSION 3.12)
project(linuxProject2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

# Find the required packages (zip and stdc++fs)
find_package(ZLIB REQUIRED)
find_package(Filesystem REQUIRED)

# Source files
set(SRCS utility.cpp execution.cpp main.cpp func.cpp)

# Build target
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem ${ZLIB_LIBRARIES})