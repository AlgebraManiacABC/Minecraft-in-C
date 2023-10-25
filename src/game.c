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

	//GLint colorVarLocation = glGetUniformLocation(shaderProgram,"varyingColor");
	//GLfloat redValue = (sinf(clock()/10000.0)+1)/2.0;
	//glUniform4f(colorVarLocation,redValue,0.0f,0.0f,0.0f);
	GLint objectColorLocation = glGetUniformLocation(shaderProgram,"objectColor");
	glUniform3f(objectColorLocation, 1.0f, 0.0f, 0.0f);

	GLint transformMatrixLocation = glGetUniformLocation(shaderProgram,"transform");

	initRenderer();
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

		renderCube(shaderProgram,cam,(vec3){0,0,0},transformMatrixLocation);
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