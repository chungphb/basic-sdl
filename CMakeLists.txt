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
set(SDL_TEST_HEADERS
	"include/core/Window.h"
	"include/core/Texture.h"
	"include/core/Button.h"
	"include/core/Timer.h"
	"include/util/TestBase.h"
)
set(SDL_TEST_SOURCES
	"src/core/Window.cpp"
	"src/core/Texture.cpp"
	"src/core/Button.cpp"
	"src/core/Timer.cpp"
	"src/util/TestBase.cpp"
)
add_library(sdl_test ${SDL_TEST_HEADERS} ${SDL_TEST_SOURCES})
target_link_libraries(sdl_test PUBLIC ${SDL2_LIBRARY} ${SDL2_image_LIBRARY} ${SDL2_ttf_LIBRARY} ${SDL2_mixer_LIBRARY})

# Lesson 1, 2, 3, 4, 5
add_executable(TestBasicSDL2 "src/test/TestBasicSDL2.cpp")
target_link_libraries(TestBasicSDL2 PUBLIC sdl_test)

# Lesson 6
add_executable(TestBasicSDL2Image "src/test/TestBasicSDL2Image.cpp")
target_link_libraries(TestBasicSDL2Image PUBLIC sdl_test)

# Lesson 7, 8, 9
add_executable(TestBasicRendering "src/test/TestBasicRendering.cpp")
target_link_libraries(TestBasicRendering PUBLIC sdl_test)

# Lesson 10
add_executable(TestBasicRenderingEx "src/test/TestBasicRenderingEx.cpp")
target_link_libraries(TestBasicRenderingEx PUBLIC sdl_test)

# Lesson 11, 12, 13, 15, 16, 17
add_executable(TestRendering "src/test/TestRendering.cpp")
target_link_libraries(TestRendering PUBLIC sdl_test)

# Lesson 14, 18
add_executable(TestRenderingEx "src/test/TestRenderingEx.cpp")
target_link_libraries(TestRenderingEx PUBLIC sdl_test)

# Lesson 19, 20
add_executable(TestController "src/test/TestController.cpp")
target_link_libraries(TestController PUBLIC sdl_test)

# Lesson 21
add_executable(TestSound "src/test/TestSound.cpp")
target_link_libraries(TestSound PUBLIC sdl_test)

# Lesson 22, 23, 24, 25
add_executable(TestTiming "src/test/TestTiming.cpp")
target_link_libraries(TestTiming PUBLIC sdl_test)

# Lesson 26, 27, 28, 29
add_executable(TestMotion "src/test/TestMotion.cpp")
target_link_libraries(TestMotion PUBLIC sdl_test)

# Lesson 30, 31
add_executable(TestScrolling "src/test/TestScrolling.cpp")
target_link_libraries(TestScrolling PUBLIC sdl_test)

# Lesson 32
add_executable(TestTextInput "src/test/TestTextInput.cpp")
target_link_libraries(TestTextInput PUBLIC sdl_test)

# Lesson 33
add_executable(TestFile "src/test/TestFile.cpp")
target_link_libraries(TestFile PUBLIC sdl_test)

# Lesson 34
add_executable(TestAudioRecording "src/test/TestAudioRecording.cpp")
target_link_libraries(TestAudioRecording PUBLIC sdl_test)

# Lesson 35, 36, 37
add_executable(TestWindow "src/test/TestWindow.cpp")
target_link_libraries(TestWindow PUBLIC sdl_test)