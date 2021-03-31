cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0074 NEW)

project(basic-sdl)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED COMPONENTS main)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_image_INCLUDE_DIR})
include_directories(${SDL2_ttf_INCLUDE_DIR})
include_directories(${SDL2_mixer_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Util classes
set(CORE_HEADERS
	"include/core/Window.h"
	"include/core/Texture.h"
	"include/core/Button.h"
	"include/core/Timer.h"
)
set(CORE_SOURCES
	"src/core/Texture.cpp"
	"src/core/Button.cpp"
	"src/core/Timer.cpp"
)
add_library(core ${CORE_HEADERS} ${CORE_SOURCES})
target_link_libraries(core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY} ${SDL2_ttf_LIBRARY})

# Lesson 1, 2, 3, 4, 5
add_executable(TestBasicSDL2 "src/test/TestBasicSDL2.cpp")
target_link_libraries(TestBasicSDL2 PRIVATE ${SDL2_LIBRARY})

# Lesson 6
add_executable(TestBasicSDL2Image "src/test/TestBasicSDL2Image.cpp")
target_link_libraries(TestBasicSDL2Image PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

# Lesson 7, 8, 9
add_executable(TestBasicRendering "src/test/TestBasicRendering.cpp")
target_link_libraries(TestBasicRendering PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

# Lesson 10
add_executable(TestBasicRenderingEx "src/test/TestBasicRenderingEx.cpp")
target_link_libraries(TestBasicRenderingEx PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

# Lesson 11, 12, 13, 15, 16, 17
add_executable(TestRendering "src/test/TestRendering.cpp")
target_link_libraries(TestRendering PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY} ${SDL2_ttf_LIBRARY})

# Lesson 14, 18
add_executable(TestRenderingEx "src/test/TestRenderingEx.cpp")
target_link_libraries(TestRenderingEx PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

# Lesson 19, 20
add_executable(TestController "src/test/TestController.cpp")
target_link_libraries(TestController PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY})

# Lesson 21
add_executable(TestSound "src/test/TestSound.cpp")
target_link_libraries(TestSound PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY} ${SDL2_mixer_LIBRARY})

# Lesson 22, 23, 24, 25
add_executable(TestTiming "src/test/TestTiming.cpp")
target_link_libraries(TestTiming PUBLIC core PRIVATE ${SDL2_LIBRARY} ${SDL2_image_LIBRARY} ${SDL2_ttf_LIBRARY})