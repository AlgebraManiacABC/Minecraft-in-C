#include "world.h"

#include <stdlib.h>

#include "raylib.h"
#include "voxel_mesh.h"
#include "raymath.h"

BlockWorld * InitWorld(int maxWidth, int maxHeight, int tempBlockLevel)
{
    BlockWorld * world = malloc(sizeof(BlockWorld));
    world->maxWidth = maxWidth;
    world->maxHeight = maxHeight;
    world->tempBlockLevel = tempBlockLevel;
    world->mesh = NULL;
    return world;
}

bool BlockIsSolid(BlockWorld * world, int x, int y, int z)
{
    if (x < 0 || x > world->maxWidth) return false;
    if (y < 0 || y > world->tempBlockLevel) return false;
    if (z < 0 || z > world->maxWidth) return false;
    return true;
}

int GetFacesInWorld(BlockWorld * world)
{
    int count = 0;
    for (int x = 0; x < world->maxWidth; x++)
    {
        for (int z = 0; z < world->maxWidth; z++)
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
    for (int x=0; x < world->maxWidth; x++)
    {
        for (int z=0; z < world->maxWidth; z++)
        {
            for (int y=0; y < world->tempBlockLevel; y++)
            {
                if (!BlockIsSolid(world, x, y, z))
                    continue;

                // build up the list of faces that this block needs
                bool faces[6] = { false, false, false, false, false, false };

                if (!BlockIsSolid(world, x+1, y, z))
                    faces[EAST] = true;

                if (!BlockIsSolid(world, x-1, y, z))
                    faces[WEST] = true;

                if (!BlockIsSolid(world, x, y+1, z))
                    faces[UP] = true;

                if (!BlockIsSolid(world, x, y-1, z))
                    faces[DOWN] = true;

                if (!BlockIsSolid(world, x, y, z+1))
                    faces[NORTH] = true;

                if (!BlockIsSolid(world, x, y, z-1))
                    faces[SOUTH] = true;

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
    DrawCubeWires((Vector3){0,0,0},1,1,1,BLACK);
    DrawCube((Vector3){0,0,0},1,1,1,RED);
    Material mat = LoadMaterialDefault();
    mat.maps[0].color = WHITE;
    mat.maps[0].texture = blockTextures.texture;
    DrawMesh(*world->mesh, mat, MatrixIdentity());
}
