#include "render.h"
#include "debug.h"

#define POS_Y_VEC3 {0,1,0}
#define NEG_Y_VEC3 {0,-1,0}
#define POS_X_VEC3 {1,0,0}
#define NEG_X_VEC3 {-1,0,0}
#define POS_Z_VEC3 {0,0,1}
#define NEG_Z_VEC3 {0,0,-1}

#define V1 {0,-1,0}
#define V2 {0,-1,1}
#define V3 {0,0,0}
#define V4 {0,0,1}
#define V5 {1,-1,0}
#define V6 {1,-1,1}
#define V7 {1,0,0}
#define V8 {1,0,1}

bool isWireframe;
GLuint cubeVertexArray;
GLuint cubeElementBuffer;
GLuint cubeVertexBuffer;
vec3 cubeVertices[] =
{
	//	Y+
	V3, POS_Y_VEC3,
	V4, POS_Y_VEC3,
	V7, POS_Y_VEC3,
	V8, POS_Y_VEC3,
	//	Y-
	V1, NEG_Y_VEC3,
	V2, NEG_Y_VEC3,
	V5, NEG_Y_VEC3,
	V6, NEG_Y_VEC3,
	//	Z+
	V1, POS_Z_VEC3,
	V3, POS_Z_VEC3,
	V5, POS_Z_VEC3,
	V7, POS_Z_VEC3,
	//	Z-
	V2, NEG_Z_VEC3,
	V4, NEG_Z_VEC3,
	V6, NEG_Z_VEC3,
	V8, NEG_Z_VEC3,
	//	X+
	V5, POS_X_VEC3,
	V6, POS_X_VEC3,
	V7, POS_X_VEC3,
	V8, POS_X_VEC3,
	//	X-
	V1, NEG_X_VEC3,
	V2, NEG_X_VEC3,
	V3, NEG_X_VEC3,
	V4, NEG_X_VEC3,
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
	//	Z+
	8,9,10,
	11,10,9,
	//	Z-
	14,13,12,
	13,14,15,
	//	X+
	18,17,16,
	17,18,19,
	//	X-
	20,21,22,
	23,22,21
};

void initRenderer(void)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1,&cubeVertexArray);
	glBindVertexArray(cubeVertexArray);
	//	Actual Vertex Data
	glGenBuffers(1,&cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_READ);

	//	Specify Vertex Positional Data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,(6*sizeof(GLfloat)),0);

	//	Specify Normal Vector Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,(6*sizeof(GLfloat)),(void*)(3*sizeof(GLfloat)));

	//	Vertex Indices
	glGenBuffers(1,&cubeElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubeElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeVertexIndices),cubeVertexIndices,GL_STATIC_READ);

	glBindVertexArray(0);

	//print1dFloatArrayAsTable((float*)cubeVertices,24,6);
}

void toggleWireframe()
{
	if(isWireframe)
	{
		isWireframe = false;
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	else
	{
		isWireframe = true;
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
}

void renderCube(GLuint shaderProgram, camera cam, vec3 voxelPosition, GLuint transformMatrixLocation)
{
	glBindVertexArray(cubeVertexArray);
	mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelMatrix,voxelPosition);
	mat4 mvpMatrix;
	setMvpMatrix(cam,modelMatrix,mvpMatrix);
	glUniformMatrix4fv(transformMatrixLocation,1,GL_FALSE,(GLfloat*)mvpMatrix);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubeElementBuffer);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
	glBindVertexArray(0);
}

void renderHex(GLuint shaderProgram, camera cam, mat4 mvpMatrix,
		GLuint hexBuffer, GLuint transformMatrixLocation)
{
	glUniformMatrix4fv(transformMatrixLocation,1,GL_FALSE,(GLfloat*)mvpMatrix);
	glBindBuffer(GL_ARRAY_BUFFER,hexBuffer);
	glDrawArrays(GL_TRIANGLE_FAN,0,8);
}

void renderUI()
{
}