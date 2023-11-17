#include <stdio.h>
#include "GL/glew.h"
#include <SDL2/SDL.h>
#include "camera.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Z 16
#define CHUNK_SIZE_Y 32

#define SEA_LEVEL 16

typedef Uint8 blockID;
typedef Uint8 blockState;

typedef struct block
{
	blockID id;

}	block_t;

typedef struct chunk
{
	//	X, Z, Y
	block_t blocks[16][16][32];

}	chunk_t;

typedef struct world
{
	chunk_t first;
	
}	*world_t;

extern world_t currentWorld;

//	Generates a simple test world (read: chunk)
void helloWorld();

/**
 * Renders the world!
 * TODO: figure out how to move to render.h/.c
 * 
 * @param shaderProgram the shader program to use while rendering (TODO: is this necessary?)
 * @param cam the camera object, so we can get view matrix
 */
void renderWorld(GLuint shaderProgram, camera_t *cam);