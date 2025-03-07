cmake_minimum_required(VERSION 3.10.0)
project(My3DEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(My3DEngine main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
target_link_libraries(My3DEngine glfw OpenGL::GL glad)