#include "render.h"

GLuint cubeElementBuffer;
GLuint cubeVertexBuffer;
GLfloat cubeVertices[] =
{
    // Front face
   -1.0, -1.0,  1.0,  0.0,  0.0,  1.0,
    1.0, -1.0,  1.0,  0.0,  0.0,  1.0,
    1.0,  1.0,  1.0,  0.0,  0.0,  1.0,
   -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,

    // Back face
   -1.0, -1.0, -1.0,  0.0,  0.0, -1.0,
    1.0, -1.0, -1.0,  0.0,  0.0, -1.0,
    1.0,  1.0, -1.0,  0.0,  0.0, -1.0,
   -1.0,  1.0, -1.0,  0.0,  0.0, -1.0,

    // Right face
    1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
    1.0, -1.0, -1.0,  1.0,  0.0,  0.0,
    1.0,  1.0, -1.0,  1.0,  0.0,  0.0,
    1.0,  1.0,  1.0,  1.0,  0.0,  0.0,

    // Left face
   -1.0, -1.0,  1.0, -1.0,  0.0,  0.0,
   -1.0, -1.0, -1.0, -1.0,  0.0,  0.0,
   -1.0,  1.0, -1.0, -1.0,  0.0,  0.0,
   -1.0,  1.0,  1.0, -1.0,  0.0,  0.0,

    // Top face
   -1.0,  1.0,  1.0,  0.0,  1.0,  0.0,
    1.0,  1.0,  1.0,  0.0,  1.0,  0.0,
    1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
   -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,

    // Bottom face
   -1.0, -1.0,  1.0,  0.0, -1.0,  0.0,
    1.0, -1.0,  1.0,  0.0, -1.0,  0.0,
    1.0, -1.0, -1.0,  0.0, -1.0,  0.0,
   -1.0, -1.0, -1.0,  0.0, -1.0,  0.0,
};
GLuint cubeVertexIndices[] =
{
    0, 1, 2, 0, 2, 3,   // Front face
    4, 5, 6, 4, 6, 7,   // Back face
    8, 9, 10, 8, 10, 11, // Right face
    12, 13, 14, 12, 14, 15, // Left face
    16, 17, 18, 16, 18, 19, // Top face
    20, 21, 22, 20, 22, 23  // Bottom face
};

void initRenderer()
{
	//	Actual Vertex Data
	glGenBuffers(1,&cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_READ);

	//	Specify Vertex Positional Data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	//	Specify Normal Vector Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_TRUE,0,(void*)(3*sizeof(GLfloat)));

	//	Vertex Indices
	glGenBuffers(1,&cubeElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubeElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeVertexIndices),cubeVertexIndices,GL_STATIC_READ);
}

void renderCube(GLuint shaderProgram, camera cam, vec3 voxelPosition, GLuint transformMatrixLocation)
{
	mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelMatrix,voxelPosition);
	mat4 mvpMatrix;
	setMvpMatrix(cam,modelMatrix,mvpMatrix);
	glUniformMatrix4fv(transformMatrixLocation,1,GL_FALSE,(GLfloat*)mvpMatrix);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubeElementBuffer);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
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