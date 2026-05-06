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

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_executable(LightWeigthGUI src/main.cpp)

    # Emscripten specific linker flags
    target_link_options(LightWeigthGUI PRIVATE
            "-sUSE_GLFW=3"
            "-sASSERTIONS=1"
#            "--shell-file=${CMAKE_SOURCE_DIR}/shell.html"
    )
else()
    add_executable(LightWeigthGUI src/main.cpp)
endif()

target_link_libraries(LightWeigthGUI raylib)