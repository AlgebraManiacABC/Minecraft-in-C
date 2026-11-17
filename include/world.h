/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H
#include "raylib.h"
#include "tree.h"

#define CHUNK_WIDTH (16)
#define MIN_BUILD_HEIGHT (0)
#define MAX_BUILD_HEIGHT (255)

typedef union
{
    struct
    {
        int x;
        int z;
    };
    long xz;
}   Coordinates_t;

typedef enum : uint8_t
{
    BLOCK_AIR = 0,
    BLOCK_STONE = 1,
    BLOCK_DIRT = 2,
    BLOCK_GRASS = 3,
    BLOCK_TYPE_COUNT = 4

}   BlockType_t;

typedef struct BlockChunk
{
    struct rb_node node;
    Coordinates_t coords; // Chunk coordinates (real / CHUNK_W/H)
    int tempSeaLevel;
    BlockType_t blocks[CHUNK_WIDTH][CHUNK_WIDTH][MAX_BUILD_HEIGHT - MIN_BUILD_HEIGHT];

    Mesh * mesh;

} BlockChunk;

#define WORLD_UP ((Vector3){0,1,0})

typedef struct BlockWorld
{
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