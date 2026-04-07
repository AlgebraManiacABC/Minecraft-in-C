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
    Color color;
    Vector2 uv;

}   VoxelMesher;

typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    UP,
    DOWN
}	BlockFace;

VoxelMesher * InitVoxelMesher(int triangles, bool useColors);

Mesh * FinalizeMesh(VoxelMesher * mesher);

void AddCube(VoxelMesher * mesher, Vector3 position, bool faces[6]);

#endif