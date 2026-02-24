#include "world.h"

#include <stdlib.h>

BlockWorld * InitWorld(int maxHeight, int maxWidth, int blockLevel)
{
    BlockWorld * world = calloc(1, sizeof(BlockWorld));
    world->maxHeight = maxHeight;
    world->maxWidth = maxWidth;
    world->blocks = calloc(maxWidth, sizeof(Block **));
    for (int x = 0; x < maxWidth; x++)
    {
        world->blocks[x] = calloc(maxWidth, sizeof(Block *));
        for (int z = 0; z < maxWidth; z++)
        {
            world->blocks[x][z] = calloc(maxHeight, sizeof(Block));
            for (int y = 0; y < blockLevel; y++)
            {
                world->blocks[x][z][y] = (Block){
                    .blockType = GRASS
                };
            }
        }
    }
    return world;
}

void DrawWorld(BlockWorld * world)
{
    // Default block to render
    Block block = {};
    // Start drawing the world at 0, 0, 0
    Position p = {0, 0, 0};
    for (p.x=0; p.x < world->maxWidth; p.x++)
    {
        for (p.z=0; p.z < world->maxWidth; p.z++)
        {
            for (p.y=0; p.y < world->maxHeight; p.y++)
            {
                if (world->blocks[p.x][p.z][p.y].blockType != AIR)
                    DrawBlock(world->blocks[p.x][p.z][p.y], p, true);
            }
        }
    }
}

void DrawBlock(Block block, Position pos, bool drawWires)
{
    Vector3 realPos = {.x = pos.x, .y = pos.y, .z = pos.z};
    DrawCube(realPos, 1, 1, 1, DARKGREEN);
    if (drawWires) DrawCubeWires(realPos, 1, 1, 1, BLACK);
}
