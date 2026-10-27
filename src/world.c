#include "../include/world.h"
#include "../include/list.h"

#include <stdlib.h>

#include "raylib.h"
#include "../include/voxel_mesh.h"
#include "raymath.h"

#define CHUNK_HEIGHT (32)
#define TEMP_BLOCK_LEVEL (16)

int ChunkCompareCoords(const struct rb_node * n, const void * key)
{
    const BlockChunk * chunk = (BlockChunk *)n;
    Coordinates_t coords = *(Coordinates_t*)key;
    return chunk->coords.xz - coords.xz;
}

int ChunkCompareChunk(const struct rb_node *n1, const struct rb_node *n2)
{
    const BlockChunk * chunk1 = (BlockChunk *)n1;
    const BlockChunk * chunk2 = (BlockChunk *)n2;
    return chunk1->coords.xz - chunk2->coords.xz;
}

BlockChunk * InitChunk(int x, int z)
{
    BlockChunk * chunk = malloc(sizeof(BlockChunk));
    if (!chunk) return NULL;
    chunk->tempBlockLevel = TEMP_BLOCK_LEVEL;
    chunk->coords.x = x;
    chunk->coords.z = z;
    return chunk;
}

BlockWorld * InitWorld(int radius)
{
    BlockWorld * world = malloc(sizeof(BlockWorld));
    if (!world) return NULL;
    world->chunkWidth = CHUNK_WIDTH;
    world->chunkHeight = CHUNK_HEIGHT;

    world->chunks = malloc(sizeof(struct rb_tree));
    rb_tree_init(world->chunks);
    for (int x = -radius+1; x < radius; x++)
    {
        for (int z = -radius+1; z < radius; z++)
        {
            BlockChunk * chunkToInsert = InitChunk(x, z);
            rb_tree_insert(world->chunks, (struct rb_node *)chunkToInsert, ChunkCompareChunk);
        }
    }

    return world;
}

bool BlockIsSolid(BlockChunk * chunk, int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_WIDTH) return false;
    if (y < 0 || y >= chunk->tempBlockLevel) return false;
    if (z < 0 || z >= CHUNK_WIDTH) return false;
    return true;
}

int GetFacesInChunk(BlockChunk * chunk)
{
    int count = 0;
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            for (int y = 0; y < CHUNK_HEIGHT; y++)
            {
                if (!BlockIsSolid(chunk, x,y,z))
                    continue;

                if (!BlockIsSolid(chunk, x+1, y, z))
                    count++;

                if (!BlockIsSolid(chunk, x-1,y,z))
                    count++;

                if (!BlockIsSolid(chunk, x,y+1,z))
                    count++;

                if (!BlockIsSolid(chunk, x,y-1,z))
                    count++;

                if (!BlockIsSolid(chunk, x,y,z+1))
                    count++;

                if (!BlockIsSolid(chunk, x,y,z-1))
                    count++;
            }
        }
    }
    return count;
}

void UpdateChunkMesh(BlockChunk * chunk)
{
    int faceCount = GetFacesInChunk(chunk);
    if (chunk->mesh)
    {
        UnloadMesh(*chunk->mesh);
        chunk->mesh = NULL;
    }
    VoxelMesher * mesher = InitVoxelMesher(faceCount * 2, true);
    for (int x=0; x < CHUNK_WIDTH; x++)
    {
        for (int z=0; z < CHUNK_WIDTH; z++)
        {
            for (int y=0; y < chunk->tempBlockLevel; y++)
            {
                if (!BlockIsSolid(chunk, x, y, z))
                    continue;

                // build up the list of faces that this block needs
                bool faces[6] = { false, false, false, false, false, false };

                if (!BlockIsSolid(chunk, x+1, y, z))
                    faces[WEST] = true;

                if (!BlockIsSolid(chunk, x-1, y, z))
                    faces[EAST] = true;

                if (!BlockIsSolid(chunk, x, y+1, z))
                    faces[UP] = true;

                if (!BlockIsSolid(chunk, x, y-1, z))
                    faces[DOWN] = true;

                if (!BlockIsSolid(chunk, x, y, z+1))
                    faces[SOUTH] = true;

                if (!BlockIsSolid(chunk, x, y, z-1))
                    faces[NORTH] = true;

                // build the faces that hit open air for this voxel block
                AddCube(mesher, (Vector3){x, y, z}, faces);
            }
        }
    }
    UploadMesh(mesher->mesh, false);
    chunk->mesh = FinalizeMesh(mesher);
}

void UpdateWorldMesh(BlockWorld * world)
{
    rb_tree_foreach(BlockChunk, chunk, world->chunks, node)
    {
        UpdateChunkMesh(chunk);
    }
}

void DrawChunk(BlockChunk * chunk, Material mat)
{
    DrawMesh(*chunk->mesh, mat, MatrixIdentity());
}

void DrawWorld(BlockWorld * world, RenderTexture blockTextures)
{
    // DrawCubeWires((Vector3){0,0,0},1,1,1,BLACK);
    // DrawCube((Vector3){0,0,0},1,1,1,RED);
    Material mat = LoadMaterialDefault();
    mat.maps[0].color = WHITE;
    mat.maps[0].texture = blockTextures.texture;
    rb_tree_foreach(BlockChunk, chunk, world->chunks, node)
    {
        DrawChunk(chunk, mat);
    }
}
