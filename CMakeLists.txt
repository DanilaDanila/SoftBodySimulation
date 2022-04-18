cmake_minimum_required(VERSION 3.15)

project(SoftBody)

set (CMAKE_CXX_STANDARD 14)

add_executable(vec2_test src/vec2_test.cpp src/vec2.h src/doctest.h)

add_executable(phys_test src/phys_test.cpp src/vec2.h src/phys.h src/doctest.h)

add_executable(sandbox src/main.cpp src/vec2.h src/renderer.h src/phys.h)

if (UNIX)
    # UNIX-like
    find_package(SDL2 REQUIRED)

    include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
    target_link_libraries(sandbox ${SDL2_LIBRARIES})
else (UNIX)
    # Windows (I hope...)
    # set(SDL2_INCLUDE_DIRS C:/Users/danila/Downloads/SDL2-devel-2.0.20-mingw.tar/SDL2-devel-2.0.20-mingw/SDL2-2.0.20/x86_64-w64-mingw32/include)
    # set(SDL2_LIBRARIES C:/Users/danila/Downloads/SDL2-devel-2.0.20-mingw.tar/SDL2-devel-2.0.20-mingw/SDL2-2.0.20/x86_64-w64-mingw32/lib)
    set(SDL2_INCLUDE_DIRS path/to/include)
    set(SDL2_LIBRARIES path/to/lib)

    include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
    target_link_libraries(sandbox -lmingw32 ${SDL2_LIBRARIES}/libSDL2main.a ${SDL2_LIBRARIES}/libSDL2.dll.a -mwindows)
endif (UNIX)
