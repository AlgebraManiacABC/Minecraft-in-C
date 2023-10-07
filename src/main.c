#include "main.h"

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

	int hex_len = 100;
	float sqrt3o2 = cosf(30);
	const GLfloat cube_vertices[] =
	{
		0.0,				0.0,
		0.0,				hex_len,
		hex_len*sqrt3o2,	hex_len/2,
		hex_len,			0.0,
		hex_len*sqrt3o2,	-hex_len/2,
		0.0,				-hex_len,
		-hex_len*sqrt3o2,	-hex_len/2,
		-hex_len,			0.0,
		-hex_len*sqrt3o2,	hex_len/2
	};
	const GLuint vertex_indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	float vertexPositions[] =
	{
		0.75f, 0.75f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f,
		-0.75f, -0.75f, 0.0f, 1.0f,
	};

	GLuint cube_vbuffer;
	glGenBuffers(1, &cube_vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,cube_vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER,0);

	GLuint cube_ibuffer;
	glGenBuffers(1, &cube_ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cube_ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);

	const char *fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n\n"
	"void main()\n"
	"{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}";
	GLuint fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	int success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		char info_log[512]={0};
		glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
		SDL_Log("Fragment Shader did not compile: %s\n",info_log);
		return EXIT_FAILURE;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		char info_log[512]={0};
		glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
		SDL_Log("Shader linking failure: %s\n",info_log);
	}
	glUseProgram(shaderProgram);

	//glBindBuffer(GL_ARRAY_BUFFER,cube_vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES,0,3);
	//SDL_Delay(5000);
	//exit(EXIT_SUCCESS);

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
						static GLuint which_vertex = 0;
						x -= (ww/2.0);
						y -= (wh/2.0);
						x /= (ww/2.0);
						y /= -(wh/2.0);
						vertexPositions[which_vertex*4] = x;
						vertexPositions[which_vertex*4 + 1] = y;
						glUseProgram(shaderProgram);
						glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
						glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);
						//SDL_Log("New vertexPositions[%d]: [%f,%f]\n",
						//	which_vertex,
						//	vertexPositions[which_vertex*4],
						//	vertexPositions[which_vertex*4 + 1]);
						glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
					}
					break;
				default:
					break;
			}
        }

		glClearColor((0xab)/255.0, 0x10/255.0, 0xfe/255.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
		glDrawArrays(GL_TRIANGLES,0,3);

		SDL_GL_SwapWindow(w);
		SDL_Delay(1000/60);
	}

	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}

