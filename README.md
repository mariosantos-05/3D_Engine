# 3D Engine

A custom 3D engine built using **C++, OpenGL, and SDL2**. This engine provides a foundation for rendering 3D graphics with real-time performance.

## Features i hope to implement
- **Cross-platform**: Works on Windows, Linux, and macOS
- **OpenGL-based rendering**: Uses modern OpenGL for efficient 3D rendering
- **SDL2 integration**: Handles windowing, input, and event management
- **Custom shader support**: Load and manage GLSL shaders
- **Camera system**: Free look & movement with keyboard and mouse
- **Basic 3D object loading**: Supports OBJ models
- **Lighting support**: Basic Phong shading implementation
- **Frame rate control**: Adjustable FPS cap with delta time

## 📸 Preview
<img src="assets/mygif.gif" width="600">

## 🛠️ Installation
### **Prerequisites**
Ensure you have the following installed:
- **C++ Compiler** (GCC/Clang/MSVC)
- **CMake** (for building the project)
- **SDL2** (Simple DirectMedia Layer)
- **GLEW** (OpenGL Extension Wrangler Library)
- **GLM** (OpenGL Mathematics)

### **Build Instructions**
```sh
# Clone the repository
git clone https://github.com/mariosantos-05/3D_Engine.git
cd 3d-engine

# Create build directory
mkdir build && cd build

# Run CMake
cmake ..

# Build the project
make -j$(nproc) # For Linux/macOS
# or
cmake --build . --config Release # For Windows

# Run the engine
./3d-engine
```

## 🎮 Controls
- `W/A/S/D` - Move the camera
- `Mouse` - Look around
- `Esc` - Exit the application

## 📂 Project Structure
```
3d-engine/
├── assets/         # Textures, models, shaders
├── src/            # Source code
│   ├── main.cpp    # Entry point
│   ├── Renderer.h  # Rendering logic
│   ├── Camera.cpp  # Camera controls
├── shaders/        # GLSL shader files
├── CMakeLists.txt  # Build configuration
└── README.md       # Project documentation
```

## 🛠️ Dependencies
- [SDL2](https://github.com/libsdl-org/SDL)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](https://github.com/g-truc/glm)

## 📜 License
This project is licensed under the **MIT License**. See `LICENSE` for details.


---
Made by [Mario Santos](https://github.com/yourusername)
