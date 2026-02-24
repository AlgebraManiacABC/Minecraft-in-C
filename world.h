/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H
#include <stdbool.h>

#include "raylib.h"

typedef struct Position
{
    int x, y, z;
}   Position;

typedef enum BlockType
{
    AIR = 0,
    GRASS,

}   BlockType;

typedef struct Block
{
    // TODO: Blocktype enum? ID?
    int blockType; // 0 == air

}   Block;

typedef struct BlockWorld
{
    // Info about the block world
    int maxHeight;
    int maxWidth; // For now, limited

    Block *** blocks; // Every block in the world

}   BlockWorld;

BlockWorld * InitWorld(int maxHeight, int maxWidth, int blockLevel);

void DrawWorld(BlockWorld *);

void DrawBlock(Block block, Position pos, bool drawWires);

#endif //MINECRAFT_IN_C_WORLD_H