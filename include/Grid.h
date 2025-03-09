#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Grid {
public:
    // Enum to specify grid direction (plane)
    enum GridDirection {
        XY_PLANE, // Default: X and Y axes
        XZ_PLANE, // X and Z axes
        YZ_PLANE  // Y and Z axes
    };

    // Constructor: Initializes the grid with a given size, step, and direction
    Grid(float gridSize, float gridStep = 1.0f, GridDirection direction = XY_PLANE)
        : gridSize(gridSize), gridStep(gridStep), direction(direction) {
        // Generate grid vertices
        std::vector<float> gridVertices = generateGridVertices(gridSize, gridStep, direction);
        vertexCount = gridVertices.size() / 3; // Each vertex has 3 components (x, y, z)

        // Set up OpenGL buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    // Destructor: Cleans up OpenGL resources
    ~Grid() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // Render the grid
    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
        shader.use();

        // Set model matrix (identity matrix for the grid)
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", glm::value_ptr(model));

        // Set view and projection matrices
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        // Bind the grid VAO and draw the grid
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertexCount);
        glBindVertexArray(0);
    }

private:
    // OpenGL buffers
    unsigned int VAO, VBO;

    // Number of vertices in the grid
    int vertexCount;

    // Grid properties
    float gridSize;
    float gridStep;
    GridDirection direction;

    // Generate grid vertices based on the specified direction
    std::vector<float> generateGridVertices(float gridSize, float gridStep, GridDirection direction) {
        std::vector<float> gridVertices;

        switch (direction) {
            case XY_PLANE:
                // Generate grid lines along the X-axis (vertical lines)
                for (float x = -gridSize; x <= gridSize; x += gridStep) {
                    gridVertices.push_back(x);
                    gridVertices.push_back(-gridSize);
                    gridVertices.push_back(0.0f);

                    gridVertices.push_back(x);
                    gridVertices.push_back(gridSize);
                    gridVertices.push_back(0.0f);
                }

                // Generate grid lines along the Y-axis (horizontal lines)
                for (float y = -gridSize; y <= gridSize; y += gridStep) {
                    gridVertices.push_back(-gridSize);
                    gridVertices.push_back(y);
                    gridVertices.push_back(0.0f);

                    gridVertices.push_back(gridSize);
                    gridVertices.push_back(y);
                    gridVertices.push_back(0.0f);
                }
                break;

            case XZ_PLANE:
                // Generate grid lines along the X-axis (vertical lines)
                for (float x = -gridSize; x <= gridSize; x += gridStep) {
                    gridVertices.push_back(x);
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(-gridSize);

                    gridVertices.push_back(x);
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(gridSize);
                }

                // Generate grid lines along the Z-axis (horizontal lines)
                for (float z = -gridSize; z <= gridSize; z += gridStep) {
                    gridVertices.push_back(-gridSize);
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(z);

                    gridVertices.push_back(gridSize);
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(z);
                }
                break;

            case YZ_PLANE:
                // Generate grid lines along the Y-axis (vertical lines)
                for (float y = -gridSize; y <= gridSize; y += gridStep) {
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(y);
                    gridVertices.push_back(-gridSize);

                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(y);
                    gridVertices.push_back(gridSize);
                }

                // Generate grid lines along the Z-axis (horizontal lines)
                for (float z = -gridSize; z <= gridSize; z += gridStep) {
                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(-gridSize);
                    gridVertices.push_back(z);

                    gridVertices.push_back(0.0f);
                    gridVertices.push_back(gridSize);
                    gridVertices.push_back(z);
                }
                break;
        }

        return gridVertices;
    }
};

#endif // GRID_H