cmake_minimum_required(VERSION 3.8)

project(basic-sdl)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED COMPONENTS main)

include_directories(${SDL2_INCLUDE_DIR})
add_executable(main src/main.cpp)
target_link_libraries(main ${SDL2_LIBRARY})