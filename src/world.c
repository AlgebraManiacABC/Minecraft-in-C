#include "world.h"
#include "assets.h"
#include "render.h"

world_t currentWorld = NULL;

void helloWorld()
{
	currentWorld = malloc(sizeof(struct world));
	for(size_t x=0; x < CHUNK_SIZE_X; x++)
	{
		for(size_t z=0; z < CHUNK_SIZE_Z; z++)
		{
			for(size_t y=0; y < CHUNK_SIZE_Y; y++)
			{
				block_t toPlace;
				if(y < SEA_LEVEL - 3)
				{
					toPlace.id = IDof("Stone");
				}
				else if(y < SEA_LEVEL)
				{
					toPlace.id = IDof("Dirt");
				}
				else if(y == SEA_LEVEL)
				{
					toPlace.id = IDof("Grass");
				}
				else
				{
					toPlace.id = 0;
				}
				currentWorld->first.blocks[x][z][y] = toPlace;
			}
		}
	}
}

void renderWorld(GLuint shaderProgram, camera_t *cam, GLint transformMatrixLocation)
{
	for(size_t x=0; x < CHUNK_SIZE_X; x++)
	{
		for(size_t z=0; z < CHUNK_SIZE_Z; z++)
		{
			for(size_t y=0; y < CHUNK_SIZE_Y; y++)
			{
				block_t currentBlock = currentWorld->first.blocks[x][z][y];
				if(currentBlock.id != 0)
				{
					renderCube(shaderProgram,cam,(vec3){x,y,z},blockTextures[currentBlock.id],transformMatrixLocation);
				}
			}
		}
	}
}