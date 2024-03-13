#include "cube.h"

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

GLuint const cubeVertexIndices[] =
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