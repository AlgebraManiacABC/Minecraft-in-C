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

void renderCube(GLuint shaderProgram, camera cam, vec3 voxelPosition);

void renderHex(GLuint shaderProgram, camera cam, mat4 mvpMatrix, GLuint hexBuffer, GLuint transformMatrixLocation);

void renderUI();

#endif