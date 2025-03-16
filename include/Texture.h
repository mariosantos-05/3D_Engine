#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>

#include "stb_image.h"

unsigned int loadTexture(const char* path);

#endif // TEXTURE_H
