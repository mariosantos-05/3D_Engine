#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FG.h"
#include "Shader.h"
#include "Setup.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Function to load a texture
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
        return 0; // Return 0 to indicate failure
    }
    stbi_image_free(data);
    return textureID;
}

int main() {
    Window win;
    if (!win.init()) return -1;

    Shader myShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");


    // Vertex data
    float vertices[] = {
        // Positions        // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f   
    };

    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int texture = loadTexture("assets/texture.png");
    if (texture == 0) {
        std::cerr << "Failed to load texture, exiting..." << std::endl;
        return -1;
    } else {
        std::cout << "Texture loaded successfully!" << std::endl;
    }

    // Set up transformation matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Pass matrices to the shader
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set texture unit
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



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
    
        myShader.setMat4("model", glm::value_ptr(cubeModel));

        myCube.Draw();

        myShader.setMat4("model", glm::value_ptr(Spheremodel));

        mySphere.draw(myShader);

        myShader.setMat4("model", glm::value_ptr(pyramidModel));

        MyPyramid.Draw();

        SDL_GL_SwapWindow(win.window);
    }
    
    
    return 0;
}