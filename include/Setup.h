#ifndef SETUP_H
#define SETUP_H


#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "Shader.h"
#include "FG.h"
#include "Texture.h"
#include "Grid.h"
#include "Orbital.h" 


class Setup
{
private:
    /* data */
    OrbitalCamera camera;
    Window win;
    Shader myShader;
    Shader gridShader;
    Grid grid;

public:
    Setup(int width, int height, const char* title);

    void Init();
    void SetShader(Shader, Shader);
    void SetGrid(Grid);
    void Shaders();
    ~Setup();
};

#endif