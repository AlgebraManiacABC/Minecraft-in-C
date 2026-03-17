#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <stddef.h>

#include "raylib.h"

typedef struct VoxelMesher
{
    Mesh * mesh;
    size_t triIndex;
    size_t vertIndex;

    Vector3 normal;
    unsigned char *color;
    Vector2 uv;

}   VoxelMesher;

#endif