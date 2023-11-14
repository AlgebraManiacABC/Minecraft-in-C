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
//
//	ABCDE
//	::::: FGHIJ
//  KLMNO
//
#define TEX_A 0,1
#define TEX_B 0.25,1
#define TEX_C 0.5,1
#define TEX_D 0.75,1
#define TEX_E 1,1
#define TEX_F 0, 0.5
#define TEX_G 0.25, 0.5
#define TEX_H 0.5, 0.5
#define TEX_I 0.75, 0.5
#define TEX_J 1, 0.5
#define TEX_K 0, 0
#define TEX_L 0.25, 0
#define TEX_M 0.5, 0
#define TEX_N 0.75, 0
#define TEX_O 1, 0

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
	//	Y+ == UP
	V3, POS_Y_VEC3, TEX_G,
	V4, POS_Y_VEC3, TEX_B,
	V7, POS_Y_VEC3, TEX_F,
	V8, POS_Y_VEC3, TEX_A,
	//	Y- == DOWN
	V1, NEG_Y_VEC3, TEX_I,
	V2, NEG_Y_VEC3, TEX_D,
	V5, NEG_Y_VEC3, TEX_J,
	V6, NEG_Y_VEC3, TEX_E,
	//	Z- == FRONT == SOUTH
	V1, NEG_Z_VEC3, TEX_M,
	V3, NEG_Z_VEC3, TEX_H,
	V5, NEG_Z_VEC3, TEX_L,
	V7, NEG_Z_VEC3, TEX_G,
	//	Z+ == BACK == NORTH
	V2, POS_Z_VEC3, TEX_N,
	V4, POS_Z_VEC3, TEX_I,
	V6, POS_Z_VEC3, TEX_O,
	V8, POS_Z_VEC3, TEX_J,
	//	X+ == EAST == RIGHT
	V5, POS_X_VEC3, TEX_N,
	V6, POS_X_VEC3, TEX_M,
	V7, POS_X_VEC3, TEX_I,
	V8, POS_X_VEC3, TEX_H,
	//	X- == WEST == LEFT
	V1, NEG_X_VEC3, TEX_K,
	V2, NEG_X_VEC3, TEX_L,
	V3, NEG_X_VEC3, TEX_F,
	V4, NEG_X_VEC3, TEX_G,
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