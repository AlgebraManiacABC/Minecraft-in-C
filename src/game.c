#include "game.h"
#include "debug.h"
#include "shaders.h"
#include "render.h"

void gameLoop(SDL_Window *w)
{
	int ww, wh;
	SDL_GetWindowSize(w,&ww,&wh);
	float aspectRatio = ww/wh;

	GLuint fragmentShader = createShader("../src/shaders/light.frag",GL_FRAGMENT_SHADER);
	if(!fragmentShader)
	{
		fprintf(stderr,"Couldn't create fragmentShader: %s\n",getError());
		return;
	}
	GLuint vertexShader = createShader("../src/shaders/transform.vert",GL_VERTEX_SHADER);
	if(!vertexShader)
	{
		fprintf(stderr,"Couldn't create vertexShader: %s\n",getError());
		return;
	}
	GLuint shaderProgram = createProgram(2,fragmentShader,vertexShader);
	if(!shaderProgram)
	{
		fprintf(stderr,"Couldn't create shader program: %s\n",getError());
		return;
	}
	glUseProgram(shaderProgram);

	//================//
	//For hex use only//
	//================//
	float hex_len = 1.0f;
	GLfloat hex_angles[] = {3.14/2, 3.14/6, -3.14/6, 3*3.14/2, 7*3.14/6, 5*3.14/6};
	GLfloat hex_vertices[] =
	{
		0.0f,				0.0f,				-0.5f,		//	0: 0,0
		hex_len*cosf(hex_angles[0]), hex_len*sinf(hex_angles[0]), 0.5f,
		hex_len*cosf(hex_angles[1]), hex_len*sinf(hex_angles[1]), 0.5f,
		hex_len*cosf(hex_angles[2]), hex_len*sinf(hex_angles[2]), 0.5f,
		hex_len*cosf(hex_angles[3]), hex_len*sinf(hex_angles[3]), 0.5f,
		hex_len*cosf(hex_angles[4]), hex_len*sinf(hex_angles[4]), 0.5f,
		hex_len*cosf(hex_angles[5]), hex_len*sinf(hex_angles[5]), 0.5f,
		hex_len*cosf(hex_angles[0]), hex_len*sinf(hex_angles[0]), 0.5f
	};
	const GLuint hex_indices[] = { 0, 1, 2, 3, 4, 5, 6, 1};

	GLuint hexBuffer;
	glGenBuffers(1, &hexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,hexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hex_vertices), hex_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	GLuint hex_ibuffer;
	glGenBuffers(1, &hex_ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,hex_ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(hex_indices),hex_indices,GL_STATIC_DRAW);
	//================//


	//GLint colorVarLocation = glGetUniformLocation(shaderProgram,"varyingColor");
	//GLfloat redValue = (sinf(clock()/10000.0)+1)/2.0;
	//glUniform4f(colorVarLocation,redValue,0.0f,0.0f,0.0f);
	GLint objectColorLocation = glGetUniformLocation(shaderProgram,"objectColor");
	glUniform3f(objectColorLocation, 1.0f, 0.0f, 0.0f);

	GLint transformMatrixLocation = glGetUniformLocation(shaderProgram,"transform");

	//	For hex use only:
	//mat4 hexModelMatrix = GLM_MAT4_IDENTITY_INIT;
	//mat4 hexRotationMatrix = GLM_MAT4_IDENTITY_INIT;
	//

	glClearColor((0xab)/255.0, 0x10/255.0, 0xfe/255.0, 1.0);
	camera cam = initCamera(aspectRatio);
	Uint32 buttonsHeld = (0b0);
	bool shouldClose = false;
	while(!shouldClose)
	{
		(void)handleEvents(&shouldClose, &cam, &buttonsHeld);
		if(shouldClose) return;
		moveCamera(&cam,buttonsHeld);

		glClear(GL_COLOR_BUFFER_BIT);

		//redValue = (sinf(clock()/10000.0)+1)/2.0;
		//glUniform4f(colorVarLocation,redValue,0.0f,0.0f,0.0f);

		//	Rotate hex (hex use only)
		//glm_rotate_x(hexRotationMatrix,0.1f,hexRotationMatrix);
		//glm_rotate_y(hexRotationMatrix,0.1f,hexRotationMatrix);
		//glm_rotate_z(hexRotationMatrix,0.1f,hexRotationMatrix);
		//glm_mat4_copy(hexRotationMatrix,hexModelMatrix);
		//

		mat4 mvpMatrix;
		//	Hex use only:
		//setMvpMatrix(cam,hexModelMatrix,mvpMatrix);
		//
		//	No model transformations:
		setMvpMatrix(cam,GLM_MAT4_IDENTITY,mvpMatrix);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,hex_ibuffer);
	    //glDrawElements(
	    //	GL_TRIANGLE_FAN,      // mode
	    //	8,    // count
	    //	GL_UNSIGNED_INT,   // type
	    //	(void*)0           // element array buffer offset
	    //);

		renderHex(shaderProgram,cam,mvpMatrix,hexBuffer,transformMatrixLocation);
		renderUI();
		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/FPS);
	}
}

int handleEvents(bool *shouldClose, camera * cam, Uint32 * buttonsHeld)
{
	SDL_Event event;
	Uint32 eventCount = 0;
	while(SDL_PollEvent(&event))
	{
		eventCount++;
		switch(event.type)
		{
			case SDL_QUIT:
				(*shouldClose) = true;
				return eventCount;
			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
						(*shouldClose) = true;
						return eventCount;
					case SDL_SCANCODE_W:
						(*buttonsHeld) |= CAMERA_MOVE_FORWARD;
						break;
					case SDL_SCANCODE_A:
						(*buttonsHeld) |= CAMERA_MOVE_LEFT;
						break;
					case SDL_SCANCODE_S:
						(*buttonsHeld) |= CAMERA_MOVE_BACKWARD;
						break;
					case SDL_SCANCODE_D:
						(*buttonsHeld) |= CAMERA_MOVE_RIGHT;
						break;
					case SDL_SCANCODE_LSHIFT:
						(*buttonsHeld) |= CAMERA_MOVE_DOWN;
						break;
					case SDL_SCANCODE_SPACE:
						(*buttonsHeld) |= CAMERA_MOVE_UP;
						break;
					case SDL_SCANCODE_LEFT:
						(*buttonsHeld) |= CAMERA_YAW_LEFT;
						break;
					case SDL_SCANCODE_RIGHT:
						(*buttonsHeld) |= CAMERA_YAW_RIGHT;
						break;
					case SDL_SCANCODE_UP:
						(*buttonsHeld) |= CAMERA_PITCH_UP;
						break;
					case SDL_SCANCODE_DOWN:
						(*buttonsHeld) |= CAMERA_PITCH_DOWN;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_W:
						(*buttonsHeld) &= ~CAMERA_MOVE_FORWARD;
						break;
					case SDL_SCANCODE_A:
						(*buttonsHeld) &= ~CAMERA_MOVE_LEFT;
						break;
					case SDL_SCANCODE_S:
						(*buttonsHeld) &= ~CAMERA_MOVE_BACKWARD;
						break;
					case SDL_SCANCODE_D:
						(*buttonsHeld) &= ~CAMERA_MOVE_RIGHT;
						break;
					case SDL_SCANCODE_LSHIFT:
						(*buttonsHeld) &= ~CAMERA_MOVE_DOWN;
						break;
					case SDL_SCANCODE_SPACE:
						(*buttonsHeld) &= ~CAMERA_MOVE_UP;
						break;
					case SDL_SCANCODE_LEFT:
						(*buttonsHeld) &= ~CAMERA_YAW_LEFT;
						break;
					case SDL_SCANCODE_RIGHT:
						(*buttonsHeld) &= ~CAMERA_YAW_RIGHT;
						break;
					case SDL_SCANCODE_UP:
						(*buttonsHeld) &= ~CAMERA_PITCH_UP;
						break;
					case SDL_SCANCODE_DOWN:
						(*buttonsHeld) &= ~CAMERA_PITCH_DOWN;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	return eventCount;
}