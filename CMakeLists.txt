cmake_minimum_required(VERSION 3.14)
project(LightWeigthGUI)

set(CMAKE_CXX_STANDARD 20)

# Raylib dependency (git submodule at external/raylib, pinned to 5.0)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

add_executable(LightWeightGUI src/main.cpp
        src/Application.cpp
        src/Application.h
        src/GUI/Widget.h
        src/GUI/Delegate.h
        src/GUI/MainWindow.h
        src/GUI/Types.h
        src/GUI/Property.h
        src/GUI/IRenderer.h
        src/GUI/InputSystem.h
        src/GUI/InputSystem.cpp
        src/GUI/Panel.h
        src/GUI/ClippingStack.h)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Emscripten specific linker flags
    target_link_options(LightWeightGUI PRIVATE
            "-sUSE_GLFW=3"           # Use the Emscripten-ported GLFW
            "-sASSERTIONS=1"         # Great for debugging GUI memory issues
            "-sALLOW_MEMORY_GROWTH=1" # Essential for retained-mode GUI growth
            "--shell-file"  "${CMAKE_SOURCE_DIR}/shell.html"
    )
    set_target_properties(LightWeightGUI PROPERTIES LINK_DEPENDS "${CMAKE_SOURCE_DIR}/shell.html")
endif()

target_link_libraries(LightWeightGUI raylib)