#include "game.h"
#include "debug.h"
#include "shaders.h"
#include "render.h"
#include "textures.h"
#include "main.h"
#include "assets.h"

void gameLoop()
{
	GLuint shaderProgram = createProgram(4,
			createShader("../src/shaders/light.frag",GL_FRAGMENT_SHADER),
			createShader("../src/shaders/texture.frag",GL_FRAGMENT_SHADER),
			createShader("../src/shaders/main.frag",GL_FRAGMENT_SHADER),
			createShader("../src/shaders/transform.vert",GL_VERTEX_SHADER));
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

	GLint modelMatrixLocation = glGetUniformLocation(shaderProgram,"modelMatrix");
	glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,(float*)GLM_MAT4_IDENTITY);

	//GLint textureLocation = glGetUniformLocation(shaderProgram,"tex");

	loadAssets("../assets/asset_list.csv");
	if(!numBlocks)
	{
		fprintf(stderr,"Error getting assets: %s\n",getError());
		return;
	}
	initRenderer();
	stbi_set_flip_vertically_on_load(true);
	GLuint stone = textureFromFile(assetFiles[IDof("Default")]);
	if(!stone)
	{
		fprintf(stderr,"Error getting stone texture: %s\n",getError());
		return;
	}
	int blockCount = 100;
	vec3 *blocks = malloc(sizeof(vec3)*blockCount);
	srand((Uint64)gameLoop + time(NULL));
	for(int i=0; i<blockCount; i++)
	{
		blocks[i][0] = rand()%20-10;
		blocks[i][1] = rand()%20-10;
		blocks[i][2] = rand()%20-10;
	}
	glClearColor((0xab)/255.0, 0x10/255.0, 0xfe/255.0, 1.0);
	camera_t *cam = initCamera();
	Uint32 buttonsHeld = (0b0);
	bool shouldClose = false;
	while(!shouldClose)
	{
		(void)handleEvents(&shouldClose, cam, &buttonsHeld);
		if(shouldClose) return;
		moveCamera(cam,buttonsHeld);
		//char buf[256]={0};
		//sprintf(buf,"XYZ: { %.2ff, %.2ff, %.2ff } || Yaw: %.2ff | Pitch %.2ff",cam.x,cam.y,cam.z,glm_deg(cam.yaw),glm_deg(cam.pitch));
		//SDL_SetWindowTitle(w,buf);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//redValue = (sinf(clock()/10000.0)+1)/2.0;
		//glUniform4f(colorVarLocation,redValue,0.0f,0.0f,0.0f);

		renderCube(shaderProgram,cam,(vec3){0,0,0},stone,transformMatrixLocation);
		for(int i=0; i<blockCount; i++)
		{
			renderCube(shaderProgram,cam,blocks[i],stone,transformMatrixLocation);
		}
		renderUI();
		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/FPS);
	}
	free(blocks);
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
					updateCameraAspectRatio(cam);
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