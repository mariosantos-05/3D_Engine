#ifndef SETUP_H
#define SETUP_H


#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "FG.h"
#include "Texture.h"
#include "Grid.h"
#include "Orbital.h" 
#include "Globals.h"


class Setup
{
private:
    /* data */
    OrbitalCamera camera;
    Window win;
public:
    Setup(int width, int height, const char* title);

    void Init();
    ~Setup();
};

#endif