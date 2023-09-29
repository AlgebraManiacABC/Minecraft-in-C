#include "main.h"

int main(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log("SDL_Init error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	int ww = 640, wh = 480;
	Uint32 wflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Window *w = SDL_CreateWindow("Title here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ww,wh,wflags);
	if(!w)
	{
		SDL_Quit();
		SDL_Log("SDL_CreateWindow error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED);
	if(!r)
	{
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("SDL_CreateRenderer error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GLContext gContext = SDL_GL_CreateContext(w);
	if(!gContext)
	{
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("SDL_GL_CreateContext error: %s\n",SDL_GetError());
	}

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		SDL_Log("glewInit failure: %s\n",glewGetErrorString(err));
		SDL_GL_DeleteContext(gContext);
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("initGL failure!\n");
	}

	//int hex_len = 100;
	//float sqrt3o2 = cosf(30);
	//const GLfloat cube_vertices[] =
	//{
	//	0.0,				0.0,
	//	0.0,				hex_len,
	//	hex_len*sqrt3o2,	hex_len/2,
	//	hex_len,			0.0,
	//	hex_len*sqrt3o2,	-hex_len/2,
	//	0.0,				-hex_len,
	//	-hex_len*sqrt3o2,	-hex_len/2,
	//	-hex_len,			0.0,
	//	-hex_len*sqrt3o2,	hex_len/2
	//};
	//const GLuint vertex_indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	////	Start of boilerplate
	//GLuint cube_vbuffer;
	//glGenBuffers(1, &cube_vbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER,cube_vbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	////
	//GLuint cube_ibuffer;
	//glGenBuffers(1, &cube_ibuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_ibuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	////	End of boilerplate

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

			glClearColor((0xab)/255.0, 0x10/255.0, 0xfe/255.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			SDL_RenderPresent(r);
			SDL_Delay(1000/60);
		}
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}