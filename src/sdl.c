#include "main.h"

int main()
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr,"SDL_Init error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	int ww = 640, wh = 480;
	Uint32 wflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Window *w = SDL_CreateWindow("Title here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ww,wh,wflags);
	if(!w)
	{
		SDL_Quit();
		fprintf(stderr,"SDL_CreateWindow error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED);
	if(!r)
	{
		SDL_DestroyWindow(w);
		SDL_Quit();
		fprintf(stderr,"SDL_CreateRenderer error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	bool should_close = false;
	while(!should_close)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					should_close = true;
					break;
				default:
					break;
			}
			SDL_RenderClear(r);
			SDL_RenderPresent(r);
			SDL_Delay(1000/60);
		}
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
}