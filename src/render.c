#include "render.h"

void renderCube(GLuint shaderProgram, camera cam, vec3 voxelPosition)
{
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