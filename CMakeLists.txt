cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0074 NEW)

project(basic-sdl)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED COMPONENTS main)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(TestSDL2 "src/TestSDL2.cpp")
target_link_libraries(TestSDL2 ${SDL2_LIBRARY})

add_executable(TestSDL2_image "src/TestSDL2_image.cpp")
target_include_directories(TestSDL2_image PRIVATE ${SDL2_image_INCLUDE_DIR})
target_link_libraries(TestSDL2_image ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

add_executable(TestTexture "src/TestTexture.cpp")
target_include_directories(TestTexture PRIVATE ${SDL2_image_INCLUDE_DIR})
target_link_libraries(TestTexture ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})