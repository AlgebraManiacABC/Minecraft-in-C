#include "game.h"
#include "debug.h"
#include "shaders.h"
#include "render.h"
#include "textures.h"
#include "main.h"
#include "assets.h"
#include "world.h"
#include "events.h"

vec3 worldUp = {0,1,0};

Uint32 gameLoop()
{
	GLuint shaderProgram = reloadShaders();
	if(!shaderProgram)
	{
		fprintf(stderr,"Couldn't load shader program: %s\n",getError());
		return STATE_ERROR;
	}

	loadAssets("../assets/asset_list.csv");
	if(!numBlocks)
	{
		fprintf(stderr,"Error getting assets: %s\n",getError());
		return STATE_ERROR;
	}
	loadFontMap("../assets/font/font_map.png");
	if(!fontMapTexture)
	{
		fprintf(stderr,"Error getting font map: %s\n",getError());
		return STATE_ERROR;
	}
	initRenderer();
	helloWorld();
	glClearColor((0x90)/255.0, 0x90/255.0, 0xfe/255.0, 1.0);
	player_t player = initPlayer((vec3){CHUNK_SIZE_X/2,SEA_LEVEL,CHUNK_SIZE_Z/2});
	Uint32 buttonsHeld = (0b0);
	Uint32 mouseState = (0b0);
	bool shouldClose = false;
	//	Rotating block variable, for debug:
	//vec3 rot = {0,0,0};
	while(!shouldClose)
	{
		if(handleEvents(&shouldClose, player, &buttonsHeld, &mouseState) < 0)
		{
			fprintf(stderr,"Error while processing events: %s\n",getError());
			return STATE_ERROR;
		}
		if(shouldClose) return STATE_QUIT;
		//SDL_WarpMouseInWindow(w,ww/2,wh/2);
		if(movePlayer(player,buttonsHeld))
		{
			fprintf(stderr,"Error while moving camera: %s\n",getError());
			return STATE_ERROR;
		}
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
		playerRenderWorld(player);
		renderUI();
		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/FPS);
	}
	// Shouldn't reach here but:
	return STATE_QUIT;
}