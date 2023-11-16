#include "render.h"
#include "debug.h"
#include "cube.h"

bool isWireframe;
GLuint cubeVertexArray;
GLuint cubeElementBuffer;
GLuint cubeVertexBuffer;

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
	glVertexAttribPointer(0,CUBE_FLOATS_PER_POS,GL_FLOAT,GL_FALSE,CUBE_SIZEOF_VERTEX,CUBE_POS_OFFSET);

	//	Specify Normal Vector Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,CUBE_FLOATS_PER_NORMAL,GL_FLOAT,GL_FALSE,CUBE_SIZEOF_VERTEX,CUBE_NORMAL_OFFSET);

	//	Specify Texture Vertex Data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,CUBE_FLOATS_PER_TEXTURE,GL_FLOAT,GL_FALSE,CUBE_SIZEOF_VERTEX,CUBE_TEXTURE_OFFSET);

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

void renderCube(GLuint shaderProgram, camera_t *cam, vec3 voxelPosition, GLuint texture, GLint transformMatrixLocation)
{
	glBindTexture(GL_TEXTURE_2D,texture);
	glBindVertexArray(cubeVertexArray);
	mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelMatrix,voxelPosition);
	mat4 mvpMatrix;
	setMvpMatrix(cam,modelMatrix,mvpMatrix);
	glUniformMatrix4fv(transformMatrixLocation,1,GL_FALSE,(GLfloat*)mvpMatrix);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubeElementBuffer);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}

void renderUI()
{
}