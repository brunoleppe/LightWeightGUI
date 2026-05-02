cmake_minimum_required(VERSION 3.14)
project(LightWeigthGUI)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.2  # Using a stable version
)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2)

add_executable(LightWeigthGUI src/main.cpp)

# SDL2 targets are exported as SDL2::SDL2 (shared) or SDL2::SDL2-static
target_link_libraries(LightWeigthGUI PRIVATE SDL2::SDL2)
