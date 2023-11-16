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

/**
 * Sets up necessary OpenGL features, generates the vertex array, and generates the cube's vertex and element buffers.
 * 
 */
void initRenderer(void);

/**
 * Toggles whether a wireframe model is rendered (default is not) 
 * 
 */
void toggleWireframe();

/**
 * Renders a single Cube using the global vertex array and other important information 
 * 
 * @param shaderProgram the shader to use while rendering (TODO: remove)
 * @param cam the camera object, for determining view matrix (TODO: maybe send in view matrix itself?)
 * @param voxelPosition the voxel position of the cube (uses the cubeWidth as offset)
 * @param texture the cube's texture
 * @param transformMatrixLocation the location of the mvpMatrix uniform in the shader program (TODO: make global?)
 */
void renderCube(GLuint shaderProgram, camera_t *cam, vec3 voxelPosition, GLuint texture, GLint transformMatrixLocation);

/**
 * Not yet implemented.
 * TODO: implement 
 * 
 */
void renderUI();

#endif