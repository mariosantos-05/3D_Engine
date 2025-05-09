#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Setup.h"
#include "Shader.h"
#include "FG.h"
#include "Texture.h"
#include "Grid.h"
#include "Orbital.h" // Include the OrbitalCamera header file

// Global camera
OrbitalCamera camera(glm::vec3(0.0f), 10.0f, -90.0f, 0.0f);

int main() {
    Window win(800, 600, "Main");
    if (!win.init()) return -1;

    // Shaders
    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    Shader gridShader("shaders/grid_vertex.glsl", "shaders/grid_fragment.glsl");

    // Objects
    Grid grid(500.0f, 1.0f, Grid::XZ_PLANE);

    // Enable OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load Textures
    unsigned int cubeTexture = loadTexture("assets/oak_veneer_01_diff_4k.jpg");
    unsigned int cubeNormalMap = loadTexture("assets/oak_veneer_01_nor_gl_1k.jpg");

    unsigned int pyramidTexture = loadTexture("assets/stonebase.png");
    unsigned int pyramidNormalMap = loadTexture("assets/stonenormal.png");

    unsigned int sphereTexture = loadTexture("assets/Metal_007_basecolor.png");
    unsigned int sphereNormalMap = loadTexture("assets/Metal_007_normal.png");

    if (!cubeTexture || !pyramidTexture || !sphereTexture) {
        std::cerr << "Texture loading failed!" << std::endl;
        return -1;
    }

    // Create 3D objects
    Cube myCube(cubeTexture, cubeNormalMap);
    Pyramid myPyramid(pyramidTexture, pyramidNormalMap);
    Sphere mySphere(0.8f, sphereTexture, sphereNormalMap);

    // Main loop
    bool running = true;
    while (running) {
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_WarpMouseInWindow(win.window, win.width / 2, win.height / 2);
                    lastX = win.width / 2;
                    lastY = win.height / 2;
                    break;
                case SDL_MOUSEMOTION:
                    handleMouseMotion(event.motion.xrel, event.motion.yrel);
                    camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);  // Update camera
                    break;
                case SDL_MOUSEWHEEL:
                    camera.ProcessMouseScroll(event.wheel.y);  // Zoom in/out
                    break;
            }
        }

        processInput(win.window);

        // Clear buffers
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Prepare camera
        glm::mat4 view = camera.GetViewMatrix();  // Get the view matrix from the orbital camera
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)win.width / win.height, 0.1f, 100.0f);

        myShader.use();
        myShader.setMat4("view", glm::value_ptr(view));
        myShader.setMat4("projection", glm::value_ptr(projection));
        myShader.setVec3("viewPos", camera.GetCameraPosition());

        float t = SDL_GetTicks() / 1000.0f;

        // Lights
        const int NR_LIGHTS = 2;
        glm::vec3 lightPositions[NR_LIGHTS] = {
            glm::vec3(-6.2f, 3.0f, 2.0f),
            glm::vec3(6.0f, -2.0f, 0.0f)
        };
        glm::vec3 lightColors[NR_LIGHTS] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };
        for (int i = 0; i < NR_LIGHTS; ++i) {
            myShader.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
            myShader.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
        }

        // Draw Cube
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::rotate(cubeModel, t, glm::vec3(0.0f, 0.0f, 1.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(0.5f, 0.5f, 0.5f));
        cubeModel = glm::translate(cubeModel, glm::vec3(1.0f, 0.5f, 0.0f));
        myShader.setMat4("model", glm::value_ptr(cubeModel));
        myCube.Draw(myShader);

        // Draw Pyramid
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::rotate(pyramidModel, t, glm::vec3(0.0f, 1.0f, 0.0f));
        pyramidModel = glm::translate(pyramidModel, glm::vec3(-1.0f, 0.5f, 0.0f));
        myShader.setMat4("model", glm::value_ptr(pyramidModel));
        myPyramid.Draw(myShader);

        // Draw Sphere
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::rotate(sphereModel, t, glm::vec3(1.0f, 1.0f, 1.0f));
        sphereModel = glm::translate(sphereModel, glm::vec3(3.0f, 0.5f, 0.0f));
        myShader.setMat4("model", glm::value_ptr(sphereModel));
        mySphere.draw(myShader);

        // Draw Grid
        grid.Draw(gridShader, view, projection);

        SDL_GL_SwapWindow(win.window);
    }

    return 0;
}
