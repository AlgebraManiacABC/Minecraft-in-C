#ifndef TEXTURES_H
#define TEXTURES_H

#include "stb_image.h"
#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct blockTexture blockTexture;

GLuint textureFromFile(const char * filename);

#endif