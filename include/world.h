/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H
#include "raylib.h"
#include "tree.h"

#define CHUNK_WIDTH (16)

typedef union
{
    struct
    {
        int x;
        int z;
    };
    long xz;
}   Coordinates_t;

typedef struct BlockChunk
{
    struct rb_node node;
    Coordinates_t coords; // Chunk coordinates (real / CHUNK_W/H)
    int tempBlockLevel; // The height up to which to generate blocks

    Mesh * mesh;


} BlockChunk;

#define WORLD_UP ((Vector3){0,1,0})

typedef struct BlockWorld
{
    // Info about the block world
    int chunkHeight;
    int chunkWidth;

    struct rb_tree * chunks;

}   BlockWorld;

int ChunkCompareCoords(const struct rb_node * n, const void * key);
int ChunkCompareChunk(const struct rb_node *n1, const struct rb_node *n2);

/**
 * Creates a new BlockWorld
 * @param radius The number of chunks beyond the spawn chunk to generate
 * @return The new BlockWorld
 */
BlockWorld * InitWorld(int radius);

void UpdateWorldMesh(BlockWorld * world);

void DrawWorld(BlockWorld *, RenderTexture blockTextures);

#endif //MINECRAFT_IN_C_WORLD_H