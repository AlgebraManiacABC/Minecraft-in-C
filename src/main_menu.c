#include "main_menu.h"
#include "main.h"
#include "cube.h"
#include "camera.h"
#include "render.h"
#include "shaders.h"
#include "assets.h"
#include "game.h"

Uint32 mainMenu(void)
{
	//vec3 origin = {0,0,0};
	//camera_t * cam = initCamera(origin);
	//	Camera faces negative z to start

	//vec3 testCubePos = {0,0,-4};

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
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//renderCube(cam,testCubePos,cubeTexture);

	bool stop = false;
	while(!stop)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					stop = true;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						stop = true;
					}
					break;
				default:
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		(void)renderText((vec2){0,0},fontMapTexture,"Hello, world!",0.01);
		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/FPS);
	}

	return STATE_QUIT;
}