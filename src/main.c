#include "main.h"
#include "shaders.h"

int main(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log("SDL_Init error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}
	int ww = 800, wh = 800;
	Uint32 wflags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Window *w = SDL_CreateWindow("Title here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ww,wh,wflags);
	if(!w)
	{
		SDL_Quit();
		SDL_Log("SDL_CreateWindow error: %s\n",SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_GLContext gContext = SDL_GL_CreateContext(w);
	if(!gContext)
	{
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("SDL_GL_CreateContext error: %s\n",SDL_GetError());
	}

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		SDL_Log("glewInit failure: %s\n",glewGetErrorString(err));
		SDL_GL_DeleteContext(gContext);
		SDL_DestroyWindow(w);
		SDL_Quit();
		SDL_Log("initGL failure!\n");
	}

	int hex_len = 1.0;
	float sqrt3o2 = cosf(30);
	GLfloat cube_vertices[] =
	{
		0.0,				0.0,		//	0: 0,0
		0.0,				hex_len,	//	1: Top
		hex_len*sqrt3o2,	hex_len/2,	//	2: Up-right
		hex_len,			0.0,		//	3: Right
		hex_len*sqrt3o2,	-hex_len/2,	//	4: Down-right
		0.0,				-hex_len,	//	5: Down
		-hex_len*sqrt3o2,	-hex_len/2,	//	6: Down-left
		-hex_len,			0.0,		//	7: Left
		-hex_len*sqrt3o2,	hex_len/2	//	8: Up-left
	};
	GLfloat hex_angles[] = {3.14/2, 3.14/6, -3.14/6, 3*3.14/2, 7*3.14/6, 5*3.14/6};
	GLfloat hex_vertices[] =
	{
		0.0,				0.0,		//	0: 0,0
		hex_len*cosf(hex_angles[0]), hex_len*sinf(hex_angles[0]),
		hex_len*cosf(hex_angles[1]), hex_len*sinf(hex_angles[1]),
		hex_len*cosf(hex_angles[2]), hex_len*sinf(hex_angles[2]),
		hex_len*cosf(hex_angles[3]), hex_len*sinf(hex_angles[3]),
		hex_len*cosf(hex_angles[4]), hex_len*sinf(hex_angles[4]),
		hex_len*cosf(hex_angles[5]), hex_len*sinf(hex_angles[5]),
		hex_len*cosf(hex_angles[0]), hex_len*sinf(hex_angles[0])
	};
	const GLuint hex_indices[] = { 0, 1, 2, 3, 4, 5, 6, 1};

	//float vertexPositions[] =
	//{
	//	0.75f, 0.75f, 0.0f, 1.0f,
	//	0.75f, -0.75f, 0.0f, 1.0f,
	//	-0.75f, -0.75f, 0.0f, 1.0f,
	//};

	GLuint cube_vbuffer;
	glGenBuffers(1, &cube_vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER,0);

	GLuint cube_ibuffer;
	glGenBuffers(1, &cube_ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hex_indices), hex_indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	GLuint hex_buffer;
	glGenBuffers(1, &hex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER,hex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hex_vertices), hex_vertices, GL_STATIC_DRAW);

	//GLuint vertex_buffer;
	//glGenBuffers(1, &vertex_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
	//glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);

	GLuint fragmentShader = mcCreateShader("../src/orange.frag",GL_FRAGMENT_SHADER);
	GLuint shaderProgram = mcCreateProgram(1,fragmentShader);

	glUseProgram(shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER,hex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);

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
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						float x,y;
						x = event.button.x;
						y = event.button.y;
						static GLuint which_vertex = 1;
						x -= (ww/2.0);
						y -= (wh/2.0);
						x /= (ww/2.0);
						y /= -(wh/2.0);
						//vertexPositions[which_vertex*4] = x;
						//vertexPositions[which_vertex*4 + 1] = y;
						hex_vertices[which_vertex*2] = x;
						hex_vertices[which_vertex*2 + 1] = y;
						if(which_vertex == 1)
						{
							hex_vertices[7*2] = x;
							hex_vertices[7*2 + 1] = y;
						}
						glUseProgram(shaderProgram);
						//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_ibuffer);
						//glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(hex_indices),vertex_indices,GL_STATIC_DRAW);
						//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_ibuffer);
						glBindBuffer(GL_ARRAY_BUFFER,hex_buffer);
						glBufferData(GL_ARRAY_BUFFER,sizeof(hex_vertices),hex_vertices,GL_STATIC_DRAW);
						glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
						SDL_Log("Vertex '%d' set to (%.2f, %.2f)\n",which_vertex,x,y);
						if(++which_vertex > 6) which_vertex = 1;
					}
					break;
				default:
					break;
			}
		}

		glClearColor((0xab)/255.0, 0x10/255.0, 0xfe/255.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindBuffer(GL_ARRAY_BUFFER,hex_buffer);
		glDrawArrays(GL_TRIANGLE_FAN,0,8);
	    //glDrawElements(
	    //	GL_TRIANGLE_FAN,      // mode
	    //	6,    // count
	    //	GL_UNSIGNED_INT,   // type
	    //	(void*)0           // element array buffer offset
	    //);

		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/60);
	}

	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}

