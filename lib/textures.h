#ifndef TEXTURES_H
#define TEXTURES_H

#include "stb_image.h"
#include <GL/glew.h>
#include <cglm/cglm.h>
#include "debug.h"

NODISCARD
/**
 * Loads the image found at the filename and stores it in a GLuint
 * 
 * @param filename The file containing the texture
 * @return a GLuint handle to the created texture, or 0 on failure.
 */
GLuint textureFromFile(const char * filename);

#endif