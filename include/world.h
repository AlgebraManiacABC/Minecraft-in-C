/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H
#include "raylib.h"

#define WORLD_UP ((Vector3){0,1,0})

typedef struct BlockChunk
{
    int x, z; // Chunk coordinates (real / CHUNK_W/H)
    int tempBlockLevel; // The height up to which to generate blocks

    Mesh * mesh;

    struct BlockChunk *n, *w, *e, *s;

} BlockChunk;

typedef struct BlockWorld
{
    // Info about the block world
    int chunkHeight;
    int chunkWidth;

    BlockChunk * spawnChunk;

}   BlockWorld;

/**
 * Creates a new BlockWorld
 * @param radius The number of chunks beyond the spawn chunk to generate
 * @return The new BlockWorld
 */
BlockWorld * InitWorld(int radius);

void UpdateWorldMesh(BlockWorld * world);

void DrawWorld(BlockWorld *, RenderTexture blockTextures);

#endif //MINECRAFT_IN_C_WORLD_H