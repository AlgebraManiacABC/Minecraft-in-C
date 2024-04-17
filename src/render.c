#include "render.h"
#include "debug.h"
#include "cube.h"
#include "shaders.h"
#include "assets.h"

bool isWireframe = false;
GLuint cubeVertexArray = 0;
GLuint cubeElementBuffer = 0;
GLuint cubeVertexBuffer = 0;
GLuint fontVertexArray = 0;
GLuint fontVertexBuffer = 0;
GLuint fontElementBuffer = 0;

void initRenderer(void)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1,&cubeVertexArray);
	glBindVertexArray(cubeVertexArray);
	//	Actual Vertex Data
	glGenBuffers(1,&cubeVertexBuffer);
	fprintf(stderr,"new VAO (cubeVertexarray) = %d\n",cubeVertexArray);
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

	//	Font Vertex/Element Buffer
	glGenVertexArrays(1,&fontVertexArray);
	fprintf(stderr,"new VAO (fontVertexarray) = %d\n",fontVertexArray);
	glBindVertexArray(fontVertexArray);

	glGenBuffers(1,&fontVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,fontVertexBuffer);

	// Specify Vertex Positional Data (Char box)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,FONT_FLOATS_PER_POS,GL_FLOAT,GL_FALSE,FONT_STRIDE,FONT_POS_OFFSET);

	// Specify Vertex Normals (all facing camera)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,FONT_FLOATS_PER_NORMAL,GL_FLOAT,GL_FALSE,FONT_STRIDE,FONT_NORMAL_OFFSET);

	// Specify Vertex Texture Data (fontMap)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,FONT_FLOATS_PER_TEXTURE,GL_FLOAT,GL_FALSE,FONT_STRIDE,FONT_TEXTURE_OFFSET);

	//	Specify Vertex Indices (Char boxes)
	glGenBuffers(1,&fontElementBuffer);
	static GLuint fontVertexIndices[] = {1,0,2, 3,1,2};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,fontElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(fontVertexIndices),fontVertexIndices,GL_STATIC_READ);

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

void renderCube(camera_t *cam, vec3 voxelPosition, GLuint texture)
{
	glBindVertexArray(cubeVertexArray);
	glBindTexture(GL_TEXTURE_2D,texture);

	mat4 modelMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_translate(modelMatrix,voxelPosition);
	glUniformMatrix4fv(mMatLocus,1,GL_FALSE,(GLfloat*)modelMatrix);

	mat4 vpMatrix;
	if(setVpMatrix(cam,vpMatrix)) return;
	glUniformMatrix4fv(vpMatLocus,1,GL_FALSE,(GLfloat*)vpMatrix);

	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
}

void renderRotatedCube(camera_t *cam, vec3 voxelPosition, GLuint texture, vec3 rot)
{
	mat4 mMatrix = GLM_MAT4_IDENTITY_INIT;
	glm_translate(mMatrix,voxelPosition);
	glm_rotate_x(mMatrix,rot[X],mMatrix);
	glm_rotate_y(mMatrix,rot[Y],mMatrix);
	glm_rotate_z(mMatrix,rot[Z],mMatrix);
	glUniformMatrix4fv(mMatLocus,1,GL_FALSE,(GLfloat*)mMatrix);

	glBindTexture(GL_TEXTURE_2D,texture);
	glBindVertexArray(cubeVertexArray);

	mat4 vpMatrix;
	if(setVpMatrix(cam,vpMatrix)) return;
	glUniformMatrix4fv(vpMatLocus,1,GL_FALSE,(GLfloat*)vpMatrix);

	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}

GLuint renderText(vec2 sp, GLuint fontMapTexture,
				const char* text, float charHeight)
{
	glBindVertexArray(fontVertexArray);
	glBindTexture(GL_TEXTURE_2D,fontMapTexture);
	glUniformMatrix4fv(vpMatLocus,1,GL_FALSE,(GLfloat*)GLM_MAT4_IDENTITY);
	glUniformMatrix4fv(mMatLocus,1,GL_FALSE,(GLfloat*)GLM_MAT4_IDENTITY);

	GLfloat texCharWidth = 1/(float)FONT_MAP_HORZ_CHARS;
	GLfloat texCharHeight = 1/(float)FONT_MAP_VERT_CHARS;
	float charWidth = (charHeight / texCharHeight) * texCharWidth;
	GLfloat charBoxVertices[] =
	{
		//	Top left:
		sp[X]-1, 1-sp[Y], 0,
			0, 0, -1,
			(1/16.0), (12/16.0),
		//	Top right:
		sp[X]-1+charWidth, 1-sp[Y], 0,
			0, 0, -1,
			(2/16.0), (12/16.0),
		//	Bottom left:
		sp[X]-1, 1-sp[Y]-charHeight, 0,
			0, 0, -1,
			(1/16.0), (11/16.0),
		//	Bottom right:
		sp[X]-1+charWidth, 1-sp[Y]-charHeight, 0,
			0, 0, -1,
			(2/16.0), (11/16.0)
	};

	int charsPrinted;
	for(charsPrinted = 0; *text && sp[X] + (charWidth*charsPrinted) < 2; text++,charsPrinted++)
	{
		//print1dFloatArrayAsTable(charBoxVertices,4,FONT_FLOATS_PER_VERTEX);
		//printf("\tfontVertexBuffer = %d\n", fontVertexBuffer);
		//puts("");
		glBufferData(GL_ARRAY_BUFFER,sizeof(charBoxVertices),charBoxVertices,GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);

		for(int i=0; i<4; i++)
		{
			charBoxVertices[X + (i * FONT_FLOATS_PER_VERTEX)] += charWidth;
		}
	}

	glBindTexture(GL_TEXTURE_2D,0);
	glBindVertexArray(0);
	return charsPrinted;
}

void renderUI()
{
	glDisable(GL_DEPTH_TEST);
	(void)renderText((vec2){0.5,0.5},fontMapTexture,"Hello, world!",0.1);
	glEnable(GL_DEPTH_TEST);
}