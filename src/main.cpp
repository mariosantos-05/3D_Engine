#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Globals.h"
#include "Setup.h"
#include "FG.h"


// Global camera
OrbitalCamera camera(glm::vec3(0.0f), 10.0f, -90.0f, 0.0f);
Setup setup(800, 600, "debug");


int main() {

    setup.init();
    
    // Create 3D objects
    Cube myCube(cubeTexture, cubeNormalMap);
    Pyramid myPyramid(pyramidTexture, pyramidNormalMap);
    Sphere mySphere(0.8f, sphereTexture, sphereNormalMap);
    
    // Main loop
    bool running = true;
    while (running) {
        setup.use();
        
        float t = SDL_GetTicks() / 1000.0f;

        // Draw Cube
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::rotate(cubeModel, t, glm::vec3(0.0f, 0.0f, 1.0f));
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
        setup.grid_draw();
        
        SDL_GL_SwapWindow(setup.win.window);
    }

    return 0;
}
