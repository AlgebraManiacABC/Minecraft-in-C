/********************************************************************************
 * world.h
 *
 ********************************************************************************/

#ifndef MINECRAFT_IN_C_WORLD_H
#define MINECRAFT_IN_C_WORLD_H

typedef struct BlockWorld
{
    // Info about the block world
    int maxHeight;
    int maxWidth; // For now, limited

    int tempBlockLevel; // The height to which to generate blocks

}   BlockWorld;

void DrawWorld(BlockWorld *);

#endif //MINECRAFT_IN_C_WORLD_H