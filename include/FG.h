#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"
struct Cube {
    GLuint VAO, VBO, EBO;
    GLuint textureID; // Texture ID for the cube

    std::vector<float> vertices = {
        // Positions          // Normals           // Texture Coords
        // Front face (Red)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  
        // Back face (Green)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  
        // Left face (Blue)
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  
        -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  
        -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  
        // Right face (Yellow)
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  
        // Top face (Magenta)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  
        // Bottom face (Cyan)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f  
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
    Cube(GLuint textureID) : textureID(textureID) {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (location = 1 in shader)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute (location = 2 in shader)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    // Translation method (updates the model matrix)
    void Translate(float x, float y, float z) {
        model = glm::translate(model, glm::vec3(x, y, z));  // Apply translation to model matrix
    }

    // Draw method (binds texture and draws the cube)
    void Draw(Shader& shader) {
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the cube's texture
        shader.setInt("texture1", 0); // Tell the shader to use texture unit 0

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
    GLuint textureID; // Texture ID for the pyramid

    std::vector<float> vertices = {
        // Positions           // Normals           // Texture Coords
        // Base face (Gray)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // Top-right
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // Top-left
        // Front face (Red)
        0.0f,  0.5f,  0.0f,  0.0f,  0.707f,  0.707f,  0.5f, 1.0f,  // Apex
        -0.5f, -0.5f,  0.5f,  0.0f,  0.707f,  0.707f,  0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.707f,  0.707f,  1.0f, 0.0f,  // Bottom-right
        // Right face (Green)
        0.0f,  0.5f,  0.0f,  0.707f,  0.707f,  0.0f,  0.5f, 1.0f,  // Apex
         0.5f, -0.5f,  0.5f,  0.707f,  0.707f,  0.0f,  0.0f, 0.0f,  // Bottom-right
         0.5f, -0.5f, -0.5f,  0.707f,  0.707f,  0.0f,  1.0f, 0.0f,  // Bottom-left
        // Back face (Blue)
        0.0f,  0.5f,  0.0f,  0.0f,  0.707f, -0.707f,  0.5f, 1.0f,  // Apex
         0.5f, -0.5f, -0.5f,  0.0f,  0.707f, -0.707f,  0.0f, 0.0f,  // Bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f,  0.707f, -0.707f,  1.0f, 0.0f,  // Bottom-left
        // Left face (Yellow)
        0.0f,  0.5f,  0.0f,  -0.707f,  0.707f,  0.0f,  0.5f, 1.0f,  // Apex
        -0.5f, -0.5f, -0.5f,  -0.707f,  0.707f,  0.0f,  0.0f, 0.0f,  // Bottom-left
        -0.5f, -0.5f,  0.5f,  -0.707f,  0.707f,  0.0f,  1.0f, 0.0f,  // Bottom-right
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
    Pyramid(GLuint textureID) : textureID(textureID) {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (location = 1 in shader)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute (location = 2 in shader)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    // Draw function
    void Draw(Shader& shader) {
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the pyramid's texture
        shader.setInt("texture1", 0); // Tell the shader to use texture unit 0

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
#define NUM_LATITUDE_SEGMENTS 80  // Number of latitude divisions
#define NUM_LONGITUDE_SEGMENTS 80 // Number of longitude divisions
struct Sphere {
    GLuint VAO, VBO, EBO;
    GLuint textureID; // Texture ID for the sphere
    float radius;

    Sphere(float r, GLuint textureID) : radius(r), textureID(textureID) {
        setupSphere();
    }

    void setupSphere() {
        std::vector<float> vertices; // Interleaved vertex data: position, normal, texture coordinates
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
    
                // Calculate texture coordinates
                float s = (float)j / NUM_LONGITUDE_SEGMENTS;
                float t = (float)i / NUM_LATITUDE_SEGMENTS;
    
                // Add the vertex position, normal, and texture coordinates
                vertices.push_back(x); // Position (x)
                vertices.push_back(y); // Position (y)
                vertices.push_back(z); // Position (z)
    
                // Normal is the normalized position
                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
                vertices.push_back(normal.x); // Normal (x)
                vertices.push_back(normal.y); // Normal (y)
                vertices.push_back(normal.z); // Normal (z)
    
                vertices.push_back(s); // Texture coordinate (s)
                vertices.push_back(t); // Texture coordinate (t)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
        // Element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
        // Vertex position attribute (location = 0 in shader)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    
        // Vertex normal attribute (location = 1 in shader)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    
        // Texture coordinate attribute (location = 2 in shader)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(const Shader& shader) {
        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, textureID); // Bind the sphere's texture
        shader.setInt("texture1", 0); // Tell the shader to use texture unit 0

        shader.use();  // Activate the shader program

        // Draw the sphere using indexed drawing
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, NUM_LATITUDE_SEGMENTS * NUM_LONGITUDE_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};