cmake_minimum_required(VERSION 3.15)

project(SoftBody)

cmake_minimum_required(VERSION 3.7)
project(SDL2Test)

find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

add_executable(sdl_test src/main.cpp src/vec2.h src/renderer.h)
target_link_libraries(sdl_test ${SDL2_LIBRARIES})
