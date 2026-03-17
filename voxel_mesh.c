#include "raylib.h"
#include "stdbool.h"

InitVoxelMesh(int triangles, bool useColors = false){

}

PushVertex(Vector3& vertex, float xOffset = 0, float yOffset = 0, float zOffset = 0){

}

void AddCube(Vector3 position, bool faces[6], int block){
	Rectangle* uvRect = &BlockColors[block];

	SetColor(BlockTints[block]);

	SetSetUV(0,0);
	//z-
	if (faces[NorthFace])
	{
		SetNormal( 0, 0, -1 );
		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 1, 1, 0);

		SetSetUV(uvRect->x, uvRect->height);
		PushVertex(position, 1, 0, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position);

		SetSetUV(uvRect->width, uvRect->y);
		PushVertex(position, 0, 1, 0);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 1, 1, 0);
	}

	// z+
	if (faces[SouthFace])
	{
		SetNormal(0, 0 ,1);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 0, 0, 1);

		SetSetUV(uvRect->x, uvRect->height);
		PushVertex(position, 1, 0, 1);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 1, 1, 1);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 0, 0, 1);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 1, 1, 1);

		SetSetUV(uvRect->width, uvRect->y);
		PushVertex(position, 0, 1, 1);
	}

	// x+
	if (faces[WestFace])
	{
		SetNormal(1, 0, 0 );
		SetSetUV(uvRect->x, uvRect->height);
 			PushVertex(position, 1, 0, 1);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 1, 0, 0);

		SetSetUV(uvRect->width, uvRect->y);
 			PushVertex(position, 1, 1, 0);

		SetSetUV(uvRect->x, uvRect->height);
 			PushVertex(position, 1, 0, 1);

		SetSetUV(uvRect->width, uvRect->y);
 			PushVertex(position, 1, 1, 0);

		SetSetUV(uvRect->x, uvRect->y);
 			PushVertex(position, 1, 1, 1);
	}

	// x-
	if (faces[EastFace])
	{
		SetNormal(-1, 0, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 0, 0, 1);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 1, 0);

		SetSetUV(uvRect->x, uvRect->height);
		PushVertex(position, 0, 0, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 0, 0, 1);

		SetSetUV(uvRect->width, uvRect->y);
		PushVertex(position, 0, 1, 1);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 1, 0);
	}

	if (faces[UpFace])
	{
		SetNormal(0, 1, 0 );

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 1, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 1, 1, 1);

		SetSetUV(uvRect->width, uvRect->y);
		PushVertex(position, 1, 1, 0);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 1, 0);

		SetSetUV(uvRect->x, uvRect->height);
		PushVertex(position, 0, 1, 1);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 1, 1, 1);
	}

	SetSetUV(0, 0);
	if (faces[DownFace])
	{
		SetNormal(0, -1, 0);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 0, 0);

		SetSetUV(uvRect->width, uvRect->y);
		PushVertex(position, 1, 0, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 1, 0, 1);

		SetSetUV(uvRect->x, uvRect->y);
		PushVertex(position, 0, 0, 0);

		SetSetUV(uvRect->width, uvRect->height);
		PushVertex(position, 1, 0, 1);

		SetSetUV(uvRect->x, uvRect->height);
		PushVertex(position, 0, 0, 1);
	}
}

inline void SetNormal(Vector3& value) { Normal = value; }
inline void SetNormal(float x, float y, float z) { Normal = Vector3{ x,y,z }; }
inline void SetSetUV(Vector2& value) { UV = value; }
inline void SetSetUV(float x, float y ) { UV = Vector2{ x,y }; }

inline void SetColor(Color& value) { VertColor = value; }
