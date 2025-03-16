cmake_minimum_required(VERSION 3.10.0)
project(My3DEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE "GLVND")

# Set the policy to handle this correctly
cmake_policy(SET CMP0072 NEW)

# Find required packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)  
    
# Add your executable
add_executable(Engine src/main.cpp src/Setup.cpp src/Orbital.cpp src/Texture.cpp src/stb_image_impl.cpp)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/shaders)
include_directories(${SDL2_INCLUDE_DIRS})  

# Add the Glad library
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)

# Link libraries
target_link_libraries(Engine glfw OpenGL::GL glad ${SDL2_LIBRARIES})  