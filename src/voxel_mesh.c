#include <stdlib.h>

#include "raylib.h"
#include "stdbool.h"
#include "../include/voxel_mesh.h"

VoxelMesher * InitVoxelMesher(int triangles, bool useColors)
{
	VoxelMesher * mesher = calloc(1, sizeof(VoxelMesher));
	Mesh * mesh = calloc(1, sizeof(Mesh));
	mesh->vertexCount = triangles * 3;
	mesh->triangleCount = triangles;
	mesh->vertices = calloc(mesh->vertexCount * 3, sizeof(float));
	mesh->normals = calloc(mesh->vertexCount * 3, sizeof(float));
	mesh->texcoords = calloc(mesh->vertexCount * 2, sizeof(float));
	mesh->colors = useColors? calloc(mesh->vertexCount, sizeof(Color)): NULL;

	mesher->mesh = mesh;

	mesher->triIndex = mesher->vertIndex = 0;
	mesher->normal = (Vector3){0, 0, 0};
	mesher->uv = (Vector2){0, 0};
	if (useColors)
	{
		mesher->color = DARKBLUE;
	}

	return mesher;
}

Mesh * FinalizeMesh(VoxelMesher * mesher)
{
	Mesh * mesh = mesher->mesh;
	free(mesher);
	return mesh;
}

void PushVertex(VoxelMesher * mesher, Vector3 vertex, float xOffset, float yOffset, float zOffset)
{
	size_t index = 0;
	if (mesher->mesh->colors)
	{
		index = (mesher->triIndex * 12) + (mesher->vertIndex * 4);
		mesher->mesh->colors[index] = mesher->color.r;
		mesher->mesh->colors[index+1] = mesher->color.g;
		mesher->mesh->colors[index+2] = mesher->color.b;
		mesher->mesh->colors[index+3] = mesher->color.a;
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

	index = (mesher->triIndex * 9) + (mesher->vertIndex * 3);
	mesher->mesh->vertices[index] = vertex.x + xOffset;
	mesher->mesh->vertices[index+1] = vertex.y + yOffset;
	mesher->mesh->vertices[index+2] = vertex.z + zOffset;

	mesher->vertIndex++;
	if (mesher->vertIndex > 2)
	{
		mesher->triIndex++;
		mesher->vertIndex = 0;
	}
}

void SetNormalV(VoxelMesher * mesher, Vector3 value) { mesher->normal = value; }
void SetNormal(VoxelMesher * mesher, float x, float y, float z) { mesher->normal = (Vector3){ x,y,z }; }

void SetUVV(VoxelMesher * mesher, Vector2 value) { mesher->uv = value; }
void SetUV(VoxelMesher * mesher, float x, float y ) { mesher->uv = (Vector2){ x,y }; }

void SetColor(VoxelMesher * mesher, Color value)
{
	mesher->color = value;
}

void AddCube(VoxelMesher * mesher, Vector3 position, bool faces[6])
{
	Rectangle * uvRect = calloc(1, sizeof(Rectangle));
	uvRect->x = 0;
	uvRect->y = 0;
	uvRect->height = 1;
	uvRect->width = 1;
	SetColor(mesher, WHITE);

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
