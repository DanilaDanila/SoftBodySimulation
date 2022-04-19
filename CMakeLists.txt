cmake_minimum_required(VERSION 3.15)

project(SoftBody)

set (CMAKE_CXX_STANDARD 14)

include_directories(thirdparty)
include_directories(engine)
include_directories(examples)

add_executable(vec2_test tests/vec2_test.cpp)

add_executable(phys_test tests/phys_test.cpp)

add_executable(sandbox examples/main.cpp)

if (UNIX)
    # UNIX-like
    find_package(SDL2 REQUIRED)

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(sandbox ${SDL2_LIBRARIES})
else (UNIX)
    include(FetchContent)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        FetchContent_Declare(
            cmake_scripts
            URL https://www.libsdl.org/release/SDL2-devel-2.0.20-mingw.tar.gz
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        FetchContent_Declare(
            cmake_scripts
            URL https://www.libsdl.org/release/SDL2-devel-2.0.20-VC.zip
        )
    endif ()
    FetchContent_Populate(cmake_scripts)

    set(SDL2_SRC ${cmake_scripts_SOURCE_DIR})

    message(STATUS "all sources: " ${SDL2_SRC})
    message(STATUS "include: " ${SDL2_SRC}/x86_64-w64-mingw32/include)
    message(STATUS "libs: " ${SDL2_SRC}/x86_64-w64-mingw32/lib/libSDL2main.a " " ${SDL2_SRC}/x86_64-w64-mingw32/lib/libSDL2.dll.a)

    configure_file(${SDL2_SRC}/x86_64-w64-mingw32/bin/SDL2.dll "./SDL2.dll" COPYONLY)

    set(SDL2_INCLUDE_DIRS ${SDL2_SRC}/x86_64-w64-mingw32/include)
    set(SDL2_LIBRARIES ${SDL2_SRC}/x86_64-w64-mingw32/lib)

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(sandbox -lmingw32 ${SDL2_LIBRARIES}/libSDL2main.a ${SDL2_LIBRARIES}/libSDL2.dll.a -mwindows)
endif (UNIX)
