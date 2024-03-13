#include "main.h"
#include "game.h"
#include "shaders.h"
#include "debug.h"
#include "window.h"
#include "assets.h"
#include "main_menu.h"

SDL_Window *w = NULL;
SDL_Renderer *r = NULL;
int ww = INITIAL_WINDOW_W, wh = INITIAL_WINDOW_H;

int mx, my;

int main(int argc, char *argv[])
{
	SDL_GLContext glContext;
	Uint32 err;
	err = initWindow(SDL_INIT_EVERYTHING,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL,
				SDL_RENDERER_ACCELERATED,
				"Cube Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				INITIAL_WINDOW_W, INITIAL_WINDOW_H, &w, &r, &glContext);
	if(err)
	{
		fprintf(stderr,"Error during intialization: %s\n",getError());
		return EXIT_FAILURE;
	}
	SDL_GetMouseState(&mx,&my);

	Uint32 state = STATE_MAIN_MENU;
	while(state != STATE_QUIT)
	{
		switch(state)
		{
			case STATE_MAIN_MENU:
				state = gameLoop();
				break;
			case STATE_NEW_WORLD:
				state = gameLoop();
				break;
			case STATE_ERROR:
				fprintf(stderr,"%s\n",getError());
			case STATE_QUIT:
				break;
			default:
				fprintf(stderr,"Unknown state in run loop: %d\n",state);
				state = STATE_QUIT;
				break;
		}
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}

