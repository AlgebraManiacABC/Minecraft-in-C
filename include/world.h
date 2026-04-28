/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H
#include "raylib.h"

#define WORLD_UP ((Vector3){0,1,0})

typedef struct BlockWorld
{
    // Info about the block world
    int maxHeight;
    int maxWidth; // For now, limited

    int tempBlockLevel; // The height to which to generate blocks

    Mesh * mesh;

}   BlockWorld;

BlockWorld * InitWorld(int maxWidth, int maxHeight, int tempBlockLevel);

void UpdateWorldMesh(BlockWorld * world);

void DrawWorld(BlockWorld *, RenderTexture blockTextures);

#endif //MINECRAFT_IN_C_WORLD_H