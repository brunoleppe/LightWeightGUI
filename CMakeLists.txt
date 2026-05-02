cmake_minimum_required(VERSION 3.14)
project(LightWeigthGUI)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.2
)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

# GLAD
FetchContent_Declare(
  glad_gen
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_GetProperties(glad_gen)
if(NOT glad_gen_POPULATED)
  FetchContent_Populate(glad_gen)
endif()

# Run GLAD generator
message(STATUS "Generating GLAD...")
execute_process(
  COMMAND python3 -m glad --generator c --spec gl --out-path ${CMAKE_BINARY_DIR}/glad --profile core --api "gl=3.3"
  WORKING_DIRECTORY ${glad_gen_SOURCE_DIR}
  RESULT_VARIABLE GLAD_GEN_RESULT
)

if (NOT GLAD_GEN_RESULT EQUAL 0)
    # Try calling via __main__.py if -m glad doesn't work
    execute_process(
      COMMAND python3 -m glad --generator c --spec gl --out-path ${CMAKE_BINARY_DIR}/glad --profile core --api "gl=3.3"
      WORKING_DIRECTORY ${glad_gen_SOURCE_DIR}
      RESULT_VARIABLE GLAD_GEN_RESULT_2
    )
    if (NOT GLAD_GEN_RESULT_2 EQUAL 0)
        message(FATAL_ERROR "Failed to generate GLAD")
    endif()
endif()

add_library(glad ${CMAKE_BINARY_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_BINARY_DIR}/glad/include)

find_package(OpenGL REQUIRED)

add_executable(LightWeigthGUI src/main.cpp)
target_link_libraries(LightWeigthGUI PRIVATE SDL2::SDL2 glad OpenGL::GL)
