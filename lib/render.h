#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>
#include "camera.h"

void initRenderer(void);

void toggleWireframe();

void renderCube(GLuint shaderProgram, camera cam, vec3 voxelPosition, GLuint texture, GLint transformMatrixLocation);

void renderUI();

#endif