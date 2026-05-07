cmake_minimum_required(VERSION 3.14)
project(LightWeigthGUI)

set(CMAKE_CXX_STANDARD 20)

# Raylib dependency
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(LightWeigthGUI src/main.cpp
        src/Application.cpp
        src/Application.h)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")

    # Emscripten specific linker flags
    target_link_options(LightWeigthGUI PRIVATE
            "-sUSE_GLFW=3"           # Use the Emscripten-ported GLFW
            "-sASSERTIONS=1"         # Great for debugging GUI memory issues
            "-sALLOW_MEMORY_GROWTH=1" # Essential for retained-mode GUI growth
    )

endif()

target_link_libraries(LightWeigthGUI raylib)