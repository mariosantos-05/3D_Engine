#ifndef WINDOW_H
#define WINDOW_H


#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

class Window {
private:
SDL_GLContext glContext;
const char* title;

public:
    SDL_Window* window;
    int width;
    int height;
    // Constructor to initialize the window parameters
    Window(int w, int h, const char* t) 
        : width(w), height(h), title(t), window(nullptr), glContext(nullptr) {}

    // Initialize the window
    bool init() {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL! Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Set OpenGL attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Create a window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
        if (!window) {
            std::cerr << "Failed to create SDL window! Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        // Create an OpenGL context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "Failed to create OpenGL context! Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Load OpenGL functions using Glad
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Enable relative mouse mode for smooth camera control
        SDL_SetRelativeMouseMode(SDL_TRUE);

        return true;
    }

    // Destructor to clean up SDL
    ~Window() {
        if (glContext) {
            SDL_GL_DeleteContext(glContext);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

    // Getter for window width
    int getWidth() const {
        return width;
    }

    // Getter for window height
    int getHeight() const {
        return height;
    }

    // Getter for window title
    const char* getTitle() const {
        return title;
    }

    // Getter for the SDL_Window pointer
    SDL_Window* getSDLWindow() const {
        return window;
    }

    // Getter for the OpenGL context
    SDL_GLContext getGLContext() const {
        return glContext;
    }
};


#endif