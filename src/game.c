#include "game.h"
#include "debug.h"
#include "shaders.h"
#include "render.h"
#include "textures.h"
#include "main.h"
#include "assets.h"
#include "world.h"

vec3 worldUp = {0,1,0};

GLuint reloadShaders()
{
	GLuint shaderProgram = createShaderProgram(2,
			"../src/shaders/main.frag",GL_FRAGMENT_SHADER,
			"../src/shaders/transform.vert",GL_VERTEX_SHADER);
	if(!shaderProgram) return 0;
	glUseProgram(shaderProgram);

	vpMatLocus = glGetUniformLocation(shaderProgram,"vpMatrix");
	mMatLocus = glGetUniformLocation(shaderProgram,"modelMatrix");
	glUniformMatrix4fv(mMatLocus,1,GL_FALSE,(float*)GLM_MAT4_IDENTITY);
	return shaderProgram;
}

void gameLoop()
{
	GLuint shaderProgram = reloadShaders();
	if(!shaderProgram)
	{
		fprintf(stderr,"Couldn't load shader program: %s\n",getError());
		return;
	}

	loadAssets("../assets/asset_list.csv");
	if(!numBlocks)
	{
		fprintf(stderr,"Error getting assets: %s\n",getError());
		return;
	}
	initRenderer();
	helloWorld();
	glClearColor((0x90)/255.0, 0x90/255.0, 0xfe/255.0, 1.0);
	camera_t *cam = initCamera();
	Uint32 buttonsHeld = (0b0);
	bool shouldClose = false;
	//	Rotating block variable, for debug:
	//vec3 rot = {0,0,0};
	while(!shouldClose)
	{
		if(handleEvents(&shouldClose, cam, &buttonsHeld) < 0)
		{
			fprintf(stderr,"Error while processing events: %s\n",getError());
			return;
		}
		if(shouldClose) return;
		moveCamera(cam,buttonsHeld);
		if(buttonsHeld & SHADER_RELOAD_REQUESTED)
		{
			GLuint tempProgram = reloadShaders();
			if(!tempProgram)
			{
				fprintf(stderr,"Couldn't reload shaders: %s\n",getError());
			}
			else
			{
				shaderProgram = tempProgram;
				buttonsHeld &= ~SHADER_RELOAD_REQUESTED;
			}
		}
		//char buf[256]={0};
		//sprintf(buf,"XYZ: { %.2ff, %.2ff, %.2ff } || Yaw: %.2ff | Pitch %.2ff",cam.x,cam.y,cam.z,glm_deg(cam.yaw),glm_deg(cam.pitch));
		//SDL_SetWindowTitle(w,buf);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rotating block functions, for debug:
		//rot[X] += 0.01f;
		//rot[Y] += 0.01f;
		//rot[Z] += 0.01f;
		//renderRotatedCube(cam,(vec3){-1,-1,-1},blockTextures[IDof("Grass")],rot);
		renderWorld(shaderProgram,cam);
		renderUI();
		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/FPS);
	}
}

int handleEvents(bool *shouldClose, camera_t *cam, Uint32 * buttonsHeld)
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
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					ww = event.window.data1;
					wh = event.window.data2;
					glViewport(0,0,ww,wh);
					if(!updateCameraAspectRatio(cam)) return -1;
					break;
				}
			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_RETURN:
						toggleWireframe();
						break;
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
					case SDL_SCANCODE_H:
						(*buttonsHeld) |= SHADER_RELOAD_REQUESTED;
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