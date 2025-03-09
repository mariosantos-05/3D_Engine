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
#include "Texture.h"
#include "Grid.h"


int main() {
    Window win;
    if (!win.init()) return -1;

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    Shader gridShader("shaders/grid_vertex.glsl", "shaders/grid_fragment.glsl");
    // Create grids in different planes
    Grid grid(500.0f, 1.0f, Grid::XZ_PLANE); // X-Z plane


    glEnable(GL_DEPTH_TEST); // Enable depth testing
glEnable(GL_BLEND);      // Enable blending
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function

    unsigned int cubeTexture = loadTexture("assets/wood.png");
    if (cubeTexture == 0) {
        std::cerr << "Failed to load texture, exiting..." << std::endl;
        return -1;
    }

    unsigned int pyramidTexture = loadTexture("assets/pedras.jpg");
    if (pyramidTexture == 0) {
        std::cerr << "Failed to load texture, exiting..." << std::endl;
        return -1;
    }

    unsigned int SphereTexture = loadTexture("assets/metal.png");
    if (SphereTexture == 0) {
        std::cerr << "Failed to load texture, exiting..." << std::endl;
        return -1;
    }
    Cube myCube(cubeTexture);
    Sphere mySphere(0.8f, SphereTexture);
    Pyramid MyPyramid(pyramidTexture);

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
        cubeModel = glm::translate(cubeModel, glm::vec3(1.0f, 0.5f, 0.0f));  // Translate cube
        myShader.setMat4("model", glm::value_ptr(cubeModel));
        myCube.Draw(myShader);

        // Apply transformation to the pyramid (separate transformation)
        glm::mat4 pyramidModel = glm::mat4(1.0f);  // Identity matrix
        pyramidModel = glm::rotate(pyramidModel,  timeInSeconds, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate pyramid
        pyramidModel = glm::translate(pyramidModel, glm::vec3(-1.0f, 0.5f, 0.0f));  // Translate pyramid
        myShader.setMat4("model", glm::value_ptr(pyramidModel));
        MyPyramid.Draw(myShader);

    
       // Apply any transformations to the model matrix for the sphere
        glm::mat4 Spheremodel = glm::mat4(1.0f);  // Identity matrix (no transformation)
        Spheremodel = glm::rotate(Spheremodel,   timeInSeconds, glm::vec3(1.0f,1.0f,1.0f));
        Spheremodel = glm::translate(Spheremodel, glm::vec3(3.0f, 0.5f, 0.0f));
        myShader.setMat4("model", glm::value_ptr(Spheremodel));
        mySphere.draw(myShader);
    // Define the number of lights
    const int NR_LIGHTS = 2;

    // Light positions and colors
    glm::vec3 lightPositions[NR_LIGHTS] = {
        glm::vec3(-6.2f, 3.0f, 2.0f),
        glm::vec3(6.0f, -2.0f, 0.0f)
    };

    glm::vec3 lightColors[NR_LIGHTS] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };

    // In your main loop, after using the shader
    myShader.use();

    // Set the light properties
    for (int i = 0; i < NR_LIGHTS; i++) {
        std::string lightPosStr = "lights[" + std::to_string(i) + "].position";
        std::string lightColorStr = "lights[" + std::to_string(i) + "].color";
        myShader.setVec3(lightPosStr, lightPositions[i]);
        myShader.setVec3(lightColorStr, lightColors[i]);
    }

    // Set the view position
    myShader.setVec3("viewPos", cameraPos);

        // Setting up a grid on the viewport
        grid.Draw(gridShader, view, projection);
        
        // Swap buffers
        SDL_GL_SwapWindow(win.window);
    }

    return 0;
}
