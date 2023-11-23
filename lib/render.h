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
 * @param cam the camera object, for determining view matrix
 * @param voxelPosition the voxel position of the cube (uses the cubeWidth as offset)
 * @param texture the cube's texture
 */
void renderCube(camera_t *cam, vec3 voxelPosition, GLuint texture);

/**
 * Renders a single (rotated) cube using the global vertex array and other info
 * 
 * @param cam the camera object, containing view and projection matrices
 * @param voxelPosition the voxel position of the cube
 * @param texture the cube's texture
 * @param rot the cube's x,y, and z rotations
 */
void renderRotatedCube(camera_t *cam, vec3 voxelPosition, GLuint texture, vec3 rot);

/**
 * Not yet implemented.
 * TODO: implement 
 * 
 */
void renderUI();

#endif