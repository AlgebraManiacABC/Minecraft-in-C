#include "../include/world.h"
#include "../include/list.h"

#include <stdlib.h>

#include "raylib.h"
#include "../include/voxel_mesh.h"
#include "raymath.h"

#define CHUNK_WIDTH (16)
#define CHUNK_HEIGHT (32)
#define TEMP_BLOCK_LEVEL (16)

BlockChunk * InitChunk(BlockChunk *n, BlockChunk *e,
                       BlockChunk *s, BlockChunk *w)
{
    BlockChunk * chunk = malloc(sizeof(BlockChunk));
    if (!chunk) return NULL;
    chunk->tempBlockLevel = TEMP_BLOCK_LEVEL;
    if ((chunk->n = n)) n->s = chunk;
    if ((chunk->e = e)) e->w = chunk;
    if ((chunk->s = s)) s->n = chunk;
    if ((chunk->w = w)) w->e = chunk;
    return chunk;
}

void SpreadSpawn(BlockChunk * chunk, int x, int z, int radius)
{
    if (!chunk) return;
    if (x < radius && !(chunk->w))
    {
        BlockChunk * w = InitChunk(NULL, chunk, NULL, NULL);
        SpreadSpawn(w, x + 1, z, radius);
    }
    if (x > -radius && !(chunk->e))
    {
        BlockChunk * e = InitChunk(NULL, NULL, NULL, chunk);
        SpreadSpawn(e, x - 1, z, radius);
    }
    if (z < radius && !(chunk->s))
    {
        BlockChunk * s = InitChunk(NULL, NULL, chunk, NULL);
        SpreadSpawn(s, x, z + 1, radius);
    }
    if (z > radius && !(chunk->n))
    {
        BlockChunk * n = InitChunk(chunk, NULL, NULL, NULL);
        SpreadSpawn(n, x, z - 1, radius);
    }
}

BlockWorld * InitWorld(int radius)
{
    BlockWorld * world = malloc(sizeof(BlockWorld));
    if (!world) return NULL;
    world->chunkWidth = CHUNK_WIDTH;
    world->chunkHeight = CHUNK_HEIGHT;

    world->spawnChunk = InitChunk(NULL, NULL, NULL, NULL);
    List_t * q = createList();
    insertNodeAtTail(q, world->spawnChunk);
    while (!listIsEmpty(q))
        BlockChunk
    SpreadSpawn(world->spawnChunk, 0, 0, radius);

    return world;
}

bool BlockIsSolid(BlockWorld * world, int x, int y, int z)
{
    if (x < 0 || x >= world->chunkWidth) return false;
    if (y < 0 || y >= world->tempBlockLevel) return false;
    if (z < 0 || z >= world->chunkWidth) return false;
    return true;
}

int GetFacesInWorld(BlockWorld * world)
{
    int count = 0;
    for (int x = 0; x < world->chunkWidth; x++)
    {
        for (int z = 0; z < world->chunkWidth; z++)
        {
            for (int y = 0; y < world->tempBlockLevel; y++)
            {
                if (!BlockIsSolid(world, x,y,z))
                    continue;

                if (!BlockIsSolid(world, x+1, y, z))
                    count++;

                if (!BlockIsSolid(world, x-1,y,z))
                    count++;

                if (!BlockIsSolid(world, x,y+1,z))
                    count++;

                if (!BlockIsSolid(world, x,y-1,z))
                    count++;

                if (!BlockIsSolid(world, x,y,z+1))
                    count++;

                if (!BlockIsSolid(world, x,y,z-1))
                    count++;
            }
        }
    }

    return count;
}

void UpdateWorldMesh(BlockWorld * world)
{
    int faceCount = GetFacesInWorld(world);
    if (world->mesh)
    {
        UnloadMesh(*world->mesh);
        world->mesh = NULL;
    }
    VoxelMesher * mesher = InitVoxelMesher(faceCount * 2, true);
    for (int x=0; x < world->chunkWidth; x++)
    {
        for (int z=0; z < world->chunkWidth; z++)
        {
            for (int y=0; y < world->tempBlockLevel; y++)
            {
                if (!BlockIsSolid(world, x, y, z))
                    continue;

                // build up the list of faces that this block needs
                bool faces[6] = { false, false, false, false, false, false };

                if (!BlockIsSolid(world, x+1, y, z))
                    faces[WEST] = true;

                if (!BlockIsSolid(world, x-1, y, z))
                    faces[EAST] = true;

                if (!BlockIsSolid(world, x, y+1, z))
                    faces[UP] = true;

                if (!BlockIsSolid(world, x, y-1, z))
                    faces[DOWN] = true;

                if (!BlockIsSolid(world, x, y, z+1))
                    faces[SOUTH] = true;

                if (!BlockIsSolid(world, x, y, z-1))
                    faces[NORTH] = true;

                // build the faces that hit open air for this voxel block
                AddCube(mesher, (Vector3){x, y, z}, faces);
            }
        }
    }
    UploadMesh(mesher->mesh, false);
    world->mesh = FinalizeMesh(mesher);
}

void DrawWorld(BlockWorld * world, RenderTexture blockTextures)
{
    // DrawCubeWires((Vector3){0,0,0},1,1,1,BLACK);
    // DrawCube((Vector3){0,0,0},1,1,1,RED);
    Material mat = LoadMaterialDefault();
    mat.maps[0].color = WHITE;
    mat.maps[0].texture = blockTextures.texture;
    DrawMesh(*world->mesh, mat, MatrixIdentity());
}
