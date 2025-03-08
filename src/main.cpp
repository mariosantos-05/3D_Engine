#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Setup.h"
#include "Shader.h"
#include "FG.h"


int main() {
    Window win;
    if (!win.init()) return -1;

    Shader myShader(vertexShaderSource, fragmentShaderSource);

   // Main loop
    bool running = true;
    while (running) {
        // Calculate delta time
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // Move the cursor to the center of the window
                    SDL_WarpMouseInWindow(win.window, win.width / 2, win.height / 2);
                    // Reset mouse position tracking
                    lastX = win.width / 2;
                    lastY = win.height / 2;
                    break;
                case SDL_MOUSEMOTION:
                    handleMouseMotion(event.motion.xrel, event.motion.yrel);
                    break;
            }
        }

        // Process keyboard input
        processInput(win.window);

        // Clear the screen
        glClearColor(0.29f, 0.29f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Sphere mySphere(0.8f);
        Cube myCube;
        Pyramid MyPyramid;


        // Use shader
        myShader.use();

        // Create the view and projection matrices
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Send view and projection matrices to shader
        myShader.setMat4("view", glm::value_ptr(view));
        myShader.setMat4("projection", glm::value_ptr(projection));
        float timeInSeconds = SDL_GetTicks() / 1000.0f;

        // Apply transformation to the cube (separate transformation)
        glm::mat4 cubeModel = glm::mat4(1.0f);  // Identity matrix
        cubeModel = glm::rotate(cubeModel,  timeInSeconds, glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate cube
        cubeModel = glm::translate(cubeModel, glm::vec3(1.0f, 0.0f, 0.0f));  // Translate cube

        // Apply transformation to the pyramid (separate transformation)
        glm::mat4 pyramidModel = glm::mat4(1.0f);  // Identity matrix
        pyramidModel = glm::rotate(pyramidModel,  timeInSeconds, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate pyramid
        pyramidModel = glm::translate(pyramidModel, glm::vec3(-1.0f, 0.0f, 0.0f));  // Translate pyramid

    
       // Apply any transformations to the model matrix for the sphere
        glm::mat4 Spheremodel = glm::mat4(1.0f);  // Identity matrix (no transformation)
        Spheremodel = glm::rotate(Spheremodel,   timeInSeconds, glm::vec3(1.0f,1.0f,1.0f));
        Spheremodel = glm::translate(Spheremodel, glm::vec3(3.0f, 0.0f, 0.0f));

        myShader.setMat4("model", glm::value_ptr(Spheremodel));

        mySphere.draw(myShader);
    
        myShader.setMat4("model", glm::value_ptr(cubeModel));

        myCube.Draw();

        myShader.setMat4("model", glm::value_ptr(pyramidModel));

        MyPyramid.Draw();
        
        // Swap buffers
        SDL_GL_SwapWindow(win.window);
    }

    return 0;
}
