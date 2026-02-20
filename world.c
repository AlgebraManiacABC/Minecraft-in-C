#include "world.h"
#include "raylib.h"

void DrawWorld(BlockWorld * world)
{
    // Start drawing the world at 0, 0, 0
    Vector3 tempPos = {0, 0, 0};
    for (tempPos.x=0; tempPos.x < world->maxWidth; tempPos.x++)
    {
        for (tempPos.z=0; tempPos.z < world->maxWidth; tempPos.z++)
        {
            for (tempPos.y=0; tempPos.y < world->tempBlockLevel; tempPos.y++)
            {
                DrawCube(tempPos, 1, 1, 1, DARKGREEN);
                DrawCubeWires(tempPos, 1, 1, 1, BLACK);
            }
        }
    }
}
