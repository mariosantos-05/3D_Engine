#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Setup.h"
#include "Shader.h"


// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 FragColor;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        FragColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 FragColor;  // Input color from vertex shader
    out vec4 FragColorOut;  // Output color to the framebuffer

    void main() {
        FragColorOut = vec4(FragColor, 1.0);  // Set output color
    }
)";

struct Cube {
    GLuint VAO, VBO, EBO;
    
    std::vector<float> vertices = {
        // Positions          // Colors
        // Front face (Red)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  
        // Back face (Green)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  
        // Left face (Blue)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  
        // Right face (Yellow)
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  
        // Top face (Magenta)
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  
        // Bottom face (Cyan)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f  
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,  // Front
        4, 5, 6, 6, 7, 4,  // Back
        8, 9,10,10,11, 8,  // Left
        12,13,14,14,15,12, // Right
        16,17,18,18,19,16, // Top
        20,21,22,22,23,20  // Bottom
    };

    glm::mat4 model = glm::mat4(1.0f); // Identity matrix

    // Constructor: Creates VAO, VBO, and EBO
    Cube() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute (location = 0 in shader)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1 in shader)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

     // Translation method (updates the model matrix)
    void Translate(float x, float y, float z) {
        model = glm::translate(model, glm::vec3(x, y, z));  // Apply translation to model matrix
    }

    void Draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Destructor to clean up buffers
    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

struct Pyramid {
    GLuint VAO, VBO, EBO;
    
    std::vector<float> vertices = {
        // Positions           // Colors
        // Base face (Gray)
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  // Bottom-left
         0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  // Bottom-right
         0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  // Top-right
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  // Top-left
        // Front face (Red)
        0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // Apex
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-right
        // Right face (Green)
        0.0f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // Apex
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Bottom-left
        // Back face (Blue)
        0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  // Apex
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Bottom-left
        // Left face (Yellow)
        0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  // Apex
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Bottom-left
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Bottom-right
    };

    std::vector<unsigned int> indices = {
        // Base
        0, 1, 2,  2, 3, 0,  // Base face
        // Front face
        4, 5, 6,
        // Right face
        7, 8, 9,
        // Back face
        10, 11, 12,
        // Left face
        13, 14, 15
    };

    // Constructor: Creates VAO, VBO, and EBO
    Pyramid() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute (location = 0 in shader)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1 in shader)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    // Draw function
    void Draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Destructor to clean up buffers
    ~Pyramid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

#define NUM_LATITUDE_SEGMENTS 30  // Number of latitude divisions
#define NUM_LONGITUDE_SEGMENTS 30 // Number of longitude divisions
struct Sphere {
    GLuint VAO, VBO, EBO;  // Added an EBO for element buffer object (indices)
    float radius;

    Sphere(float r) : radius(r) {
        setupSphere();
    }

    void setupSphere() {
        std::vector<glm::vec3> vertices;
        std::vector<GLuint> indices;

        // Create vertices for the sphere using spherical coordinates
        for (int i = 0; i <= NUM_LATITUDE_SEGMENTS; ++i) {
            float phi = glm::radians(i * 180.0f / NUM_LATITUDE_SEGMENTS); // Polar angle
            for (int j = 0; j <= NUM_LONGITUDE_SEGMENTS; ++j) {
                float theta = glm::radians(j * 360.0f / NUM_LONGITUDE_SEGMENTS); // Azimuthal angle

                // Convert spherical coordinates to Cartesian coordinates
                float x = radius * sin(phi) * cos(theta);
                float y = radius * sin(phi) * sin(theta);
                float z = radius * cos(phi);

                // Calculate color based on the spherical coordinates (phi, theta)
                float red = (sin(phi) + 1.0f) / 2.0f;  // Color variation based on latitude (phi)
                float green = (cos(theta) + 1.0f) / 2.0f;  // Color variation based on longitude (theta)
                float blue = (sin(theta) + 1.0f) / 2.0f;  // Another variation for a unique effect

                glm::vec3 color(red, green, blue);  // Color based on spherical coordinates

                // Add the vertex position and its color
                vertices.push_back(glm::vec3(x, y, z));  // Position
                vertices.push_back(color);  // Color
            }
        }

        // Create indices for the sphere's faces (using triangles)
        for (int i = 0; i < NUM_LATITUDE_SEGMENTS; ++i) {
            for (int j = 0; j < NUM_LONGITUDE_SEGMENTS; ++j) {
                int first = i * (NUM_LONGITUDE_SEGMENTS + 1) + j;
                int second = first + NUM_LONGITUDE_SEGMENTS + 1;

                // Create two triangles for each segment (top and bottom triangles)
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        // Create VAO, VBO, and EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2, &vertices[0], GL_STATIC_DRAW);

        // Element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        // Vertex position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(const Shader& shader) {
        shader.use();  // Activate the shader program

        // Draw the sphere using indexed drawing
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, NUM_LATITUDE_SEGMENTS * NUM_LONGITUDE_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};

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
