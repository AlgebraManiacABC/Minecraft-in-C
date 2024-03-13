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

extern GLfloat const cubeVertices[192];
extern GLuint const cubeVertexIndices[36];

#endif