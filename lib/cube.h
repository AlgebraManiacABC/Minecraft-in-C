#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <cglm/cglm.h>

//	Normals
#define POS_Y_VEC3 0,1,0
#define NEG_Y_VEC3 0,-1,0
#define POS_X_VEC3 1,0,0
#define NEG_X_VEC3 -1,0,0
#define POS_Z_VEC3 0,0,1
#define NEG_Z_VEC3 0,0,-1

//	Vertex positions
#define V1 0,-1,0
#define V2 0,-1,1
#define V3 0,0,0
#define V4 0,0,1
#define V5 1,-1,0
#define V6 1,-1,1
#define V7 1,0,0
#define V8 1,0,1

//	Texture coordinates
#define TEX_UL 0,1
#define TEX_UR 1,1
#define TEX_DL 0,0
#define TEX_DR 1,0

//	Cube vertices metadata
#define CUBE_POS_OFFSET (void*)(0)
#define CUBE_FLOATS_PER_POS (3)
#define CUBE_NORMAL_OFFSET (void*)(sizeof(GLfloat)*CUBE_FLOATS_PER_POS)
#define CUBE_FLOATS_PER_NORMAL (3)
#define CUBE_TEXTURE_OFFSET (void*)(sizeof(GLfloat)*(CUBE_FLOATS_PER_NORMAL+CUBE_FLOATS_PER_POS))
#define CUBE_FLOATS_PER_TEXTURE (2)
#define CUBE_SIZEOF_VERTEX (sizeof(GLfloat)*CUBE_FLOATS_PER_VERTEX)
#define CUBE_FLOATS_PER_VERTEX (CUBE_FLOATS_PER_NORMAL+CUBE_FLOATS_PER_POS+CUBE_FLOATS_PER_TEXTURE)

GLfloat const cubeVertices[] =
{
	//	Y+
	V3, POS_Y_VEC3, TEX_DR,
	V4, POS_Y_VEC3, TEX_UR,
	V7, POS_Y_VEC3, TEX_DL,
	V8, POS_Y_VEC3, TEX_UL,
	//	Y-
	V1, NEG_Y_VEC3, TEX_DL,
	V2, NEG_Y_VEC3, TEX_UL,
	V5, NEG_Y_VEC3, TEX_DR,
	V6, NEG_Y_VEC3, TEX_UR,
	//	Z-
	V1, NEG_Z_VEC3, TEX_DR,
	V3, NEG_Z_VEC3, TEX_UR,
	V5, NEG_Z_VEC3, TEX_DL,
	V7, NEG_Z_VEC3, TEX_UL,
	//	Z+
	V2, POS_Z_VEC3, TEX_DL,
	V4, POS_Z_VEC3, TEX_UL,
	V6, POS_Z_VEC3, TEX_DR,
	V8, POS_Z_VEC3, TEX_UR,
	//	X+
	V5, POS_X_VEC3, TEX_DR,
	V6, POS_X_VEC3, TEX_DL,
	V7, POS_X_VEC3, TEX_UR,
	V8, POS_X_VEC3, TEX_UL,
	//	X-
	V1, NEG_X_VEC3, TEX_DL,
	V2, NEG_X_VEC3, TEX_DR,
	V3, NEG_X_VEC3, TEX_UL,
	V4, NEG_X_VEC3, TEX_UR,
};
GLuint cubeVertexIndices[] =
{
	//	Going CLOCKWISE (GL_CW)
	//	Y+
	0,1,2,
	3,2,1,
	//	Y-
	6,5,4,
	5,6,7,
	//	Z-
	8,9,10,
	11,10,9,
	//	Z+
	14,13,12,
	13,14,15,
	//	X+
	18,17,16,
	17,18,19,
	//	X-
	20,21,22,
	23,22,21
};

#endif