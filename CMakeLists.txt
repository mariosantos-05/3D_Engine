cmake_minimum_required(VERSION 3.10.0)
project(My3DEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(My3DEngine main.cpp)
target_link_libraries(My3DEngine glfw OpenGL::GL)