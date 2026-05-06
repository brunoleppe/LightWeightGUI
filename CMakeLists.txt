cmake_minimum_required(VERSION 3.14)
project(LightWeightGUI)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
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

execute_process(
  COMMAND python3 -m glad --generator c --spec gl --out-path ${CMAKE_BINARY_DIR}/glad --profile core --api "gl=3.3"
  WORKING_DIRECTORY ${glad_gen_SOURCE_DIR}
)

add_library(glad ${CMAKE_BINARY_DIR}/glad/src/glad.c
        src/ECS/Registry.cpp
        src/ECS/Registry.h
        src/GUI/GUIPrimitives.h
        src/GUI/Widget.h
        src/GUI/Delegate.h
        src/ECS/Engine.h)
target_include_directories(glad PUBLIC ${CMAKE_BINARY_DIR}/glad/include)

find_package(OpenGL REQUIRED)

add_executable(LightWeightGUI
    src/main.cpp 
    src/Application.cpp
    src/Shader.cpp
)
target_include_directories(LightWeightGUI PRIVATE src src/ECS src/GUI)
if(WIN32)
target_link_libraries(LightWeightGUI PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        glad
        OpenGL::GL)

  add_custom_command(TARGET LightWeightGUI POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:SDL2::SDL2>
          $<TARGET_FILE_DIR:LightWeightGUI>
  )

else ()
  target_link_libraries(LightWeightGUI PRIVATE
          SDL2::SDL2
          glad
          OpenGL::GL)
endif ()