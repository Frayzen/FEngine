cmake_minimum_required(VERSION 3.5)
project(engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=c++20 -Wno-old-style-cast")

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

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Ensure to link against both gtest and gtest_main
link_libraries(GLEW glfw OpenGL::GL imgui glm::glm assimp gtest gtest_main)

enable_testing()

file(GLOB_RECURSE sources src/*.cc)

add_executable(engine ${sources})

target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(fem_2d_mesh_test src/fem/fem_2d_mesh_test.cc)

target_link_libraries(fem_2d_mesh_test gtest gtest_main)

add_test(NAME FEM2DMeshTest COMMAND fem_2d_mesh_test)

