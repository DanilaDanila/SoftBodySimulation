cmake_minimum_required(VERSION 3.15)

project(SoftBody)

set (CMAKE_CXX_STANDARD 14)

if (UNIX)
    # UNIX-like
    find_package(SDL2 REQUIRED)
    include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
else (UNIX)
    # Windows (I hope...)

endif (UNIX)

add_executable(vec2_test src/vec2_test.cpp src/vec2.h src/doctest.h)

add_executable(phys_test src/phys_test.cpp src/vec2.h src/phys.h src/doctest.h)

add_executable(sandbox src/main.cpp src/vec2.h src/renderer.h src/phys.h)
target_link_libraries(sandbox ${SDL2_LIBRARIES})
