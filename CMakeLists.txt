cmake_minimum_required(VERSION 3.10.0)
project(My3DEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE "GLVND")
# Set the policy to handle this correctly
cmake_policy(SET CMP0072 NEW)



find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(Engine src/main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/include)


add_library(glad STATIC ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)

target_link_libraries(Engine glfw OpenGL::GL glad)