cmake_minimum_required(VERSION 3.5)
project(engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -Wno-error=old-style-cast")

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(imgui REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${OPENGL_GLU_INCLUDE_DIR})
include_directories(${imgui_DIR})

link_libraries(GLEW)
link_libraries(glfw)
link_libraries(OpenGL::GL)
link_libraries(imgui)
link_libraries(glm::glm)
link_libraries(assimp)

file(GLOB_RECURSE sources src/*.cc)

add_executable(engine ${sources})

target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/src)
