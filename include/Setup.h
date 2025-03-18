#ifndef SETUP_H
#define SETUP_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Texture.h"
#include "Shader.h"
#include "Grid.h"
#include "Orbital.h"
#include "Window.h"
#include "Globals.h"


extern unsigned int cubeTexture;
extern unsigned int cubeNormalMap;
extern unsigned int pyramidTexture;
extern unsigned int pyramidNormalMap;
extern unsigned int sphereTexture;
extern unsigned int sphereNormalMap;

class Setup {
    public:
    Setup(int width, int height, const char* title);
    ~Setup();
    
    bool init();
    void use();
    void processInput();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    
    void grid_draw();
    Window win;

    
private:
    void handleMouseMotion(int xrel, int yrel);
    void handleMouseWheel(int yoffset);
    
    OrbitalCamera camera;
    


    float deltaTime;
    float lastFrame;
    float lastX, lastY;
    bool firstMouse;

    glm::mat4 view;
    glm::mat4 projection;


    void loadTextures();
    void setupLights();
};

#endif // SETUP_H
