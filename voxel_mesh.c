#include <stdlib.h>

#include "raylib.h"
#include "stdbool.h"
#include "voxel_mesh.h"

VoxelMesher * InitVoxelMesher(int triangles, bool useColors)
{
	VoxelMesher * mesher = calloc(1, sizeof(VoxelMesher));
	Mesh * mesh = calloc(1, sizeof(Mesh));
	mesh->vertexCount = triangles * 6;
	mesh->triangleCount = triangles * 2;
	mesh->vertices = calloc(mesh->vertexCount * 3, sizeof(float));
	mesh->normals = calloc(mesh->vertexCount * 3, sizeof(float));
	mesh->texcoords = calloc(mesh->vertexCount * 2, sizeof(float));
	mesh->colors = useColors? calloc(mesh->vertexCount, sizeof(Color)): NULL;
	mesher->mesh = mesh;

	mesher->triIndex = mesher->vertIndex = 0;
	mesher->normal = (Vector3){0, 0, 0};
	mesher->uv = (Vector2){0, 0};
	mesher->color[0] = WHITE.r;
	mesher->color[1] = WHITE.g;
	mesher->color[2] = WHITE.b;
	mesher->color[3] = WHITE.a;

	return mesher;
}

inline void PushVertex(VoxelMesher * mesher, Vector3 vertex, float xOffset, float yOffset, float zOffset)
{
	size_t index = 0;
	if (mesher->mesh->colors)
	{
		index = (mesher->triIndex * 12) + (mesher->vertIndex * 4);
		for (int i=0; i<4; i++)
		{
			mesher->mesh->colors[index+i] = mesher->color[i];
		}
	}
	if (mesher->mesh->normals)
	{
		index = (mesher->triIndex * 9) + (mesher->vertIndex * 3);
		mesher->mesh->normals[index] = mesher->normal.x;
		mesher->mesh->normals[index+1] = mesher->normal.y;
		mesher->mesh->normals[index+2] = mesher->normal.z;
	}
	if (mesher->mesh->texcoords)
	{
		index = (mesher->triIndex * 6) + (mesher->vertIndex * 2);
		mesher->mesh->texcoords[index] = mesher->uv.x;
		mesher->mesh->texcoords[index+1] = mesher->uv.y;
	}
	mesher->vertIndex++;
	if (mesher->vertIndex > 2)
	{
		mesher->triIndex++;
		mesher->vertIndex = 0;
	}
}

inline void SetNormalV(VoxelMesher * mesher, Vector3 value) { mesher->normal = value; }
inline void SetNormal(VoxelMesher * mesher, float x, float y, float z) { mesher->normal = (Vector3){ x,y,z }; }

inline void SetUVV(VoxelMesher * mesher, Vector2 value) { mesher->uv = value; }
inline void SetUV(VoxelMesher * mesher, float x, float y ) { mesher->uv = (Vector2){ x,y }; }

inline void SetColor(VoxelMesher * mesher, Color value)
{
	mesher->color[0] = value.r;
	mesher->color[1] = value.g;
	mesher->color[2] = value.b;
	mesher->color[3] = value.a;
}

typedef enum
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	UP,
	DOWN
}	BlockFace;

void AddCube(VoxelMesher * mesher, Vector3 position, bool faces[6])
{
	Rectangle * uvRect = calloc(1, sizeof(Rectangle));
	uvRect->x = DARKGREEN.r;
	uvRect->y = DARKGREEN.g;
	uvRect->height = DARKGREEN.b;
	uvRect->width = DARKGREEN.a;
	SetColor(mesher, DARKGREEN);

	SetUV(mesher, 0,0);
	//z-
	if (faces[NORTH])
	{
		SetNormal(mesher,  0, 0, -1 );
		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 0);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 1, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->height);
		PushVertex(mesher, position, 1, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->y);
		PushVertex(mesher, position, 0, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 1, 1, 0);
	}

	// z+
	if (faces[SOUTH])
	{
		SetNormal(mesher, 0, 0 ,1);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->height);
		PushVertex(mesher, position, 1, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 1, 1, 1);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 1, 1, 1);

		SetUV(mesher, uvRect->width, uvRect->y);
		PushVertex(mesher, position, 0, 1, 1);
	}

	// x+
	if (faces[WEST])
	{
		SetNormal(mesher, 1, 0, 0 );
		SetUV(mesher, uvRect->x, uvRect->height);
 			PushVertex(mesher, position, 1, 0, 1);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 1, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->y);
 			PushVertex(mesher, position, 1, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->height);
 			PushVertex(mesher, position, 1, 0, 1);

		SetUV(mesher, uvRect->width, uvRect->y);
 			PushVertex(mesher, position, 1, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->y);
 			PushVertex(mesher, position, 1, 1, 1);
	}

	// x-
	if (faces[EAST])
	{
		SetNormal(mesher, -1, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->height);
		PushVertex(mesher, position, 0, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 0, 0, 1);

		SetUV(mesher, uvRect->width, uvRect->y);
		PushVertex(mesher, position, 0, 1, 1);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 1, 0);
	}

	if (faces[UP])
	{
		SetNormal(mesher, 0, 1, 0 );

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 1, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 1, 1, 1);

		SetUV(mesher, uvRect->width, uvRect->y);
		PushVertex(mesher, position, 1, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 1, 0);

		SetUV(mesher, uvRect->x, uvRect->height);
		PushVertex(mesher, position, 0, 1, 1);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 1, 1, 1);
	}

	SetUV(mesher, 0, 0);
	if (faces[DOWN])
	{
		SetNormal(mesher, 0, -1, 0);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->y);
		PushVertex(mesher, position, 1, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 1, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->y);
		PushVertex(mesher, position, 0, 0, 0);

		SetUV(mesher, uvRect->width, uvRect->height);
		PushVertex(mesher, position, 1, 0, 1);

		SetUV(mesher, uvRect->x, uvRect->height);
		PushVertex(mesher, position, 0, 0, 1);
	}
}
