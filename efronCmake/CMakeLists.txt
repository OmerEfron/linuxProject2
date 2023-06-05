cmake_minimum_required(VERSION 3.13)  
project(linux_second_assignment)           
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20) 

find_library(ZIPLIB zip)

set(SOURCE_FILES main.cpp utility.cpp execution.cpp func.cpp)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(main ${SOURCE_FILES} ${SRC_FILES})

target_link_libraries(main PUBLIC ${ZIPLIB})

target_include_directories(main PUBLIC  
                           "${CMAKE_CURRENT_SOURCE_DIR}/include/"
                           "${CMAKE_CURRENT_SOURCE_DIR}/My_lib/include/"
                           "/usr/local/include/"
                           )
                           
configure_file(flightScanner.sh ${CMAKE_CURRENT_BINARY_DIR}/script.sh COPYONLY)
