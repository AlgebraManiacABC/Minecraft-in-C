#include "shaders.h"
#include <cglm/cglm.h>

GLint vpMatLocus = 0;
GLint mMatLocus = 0;

GLuint createShaderProgram(size_t shaderCount, ...)
{
	if(shaderCount <= 0)
	{
		setError(ERR_MESG,"Could not create shader program: Shader count was 0");
		return 0;
	}
	GLuint shaderProgram = glCreateProgram();
	va_list args;
	va_start(args, shaderCount);
	GLuint * shaders = calloc(shaderCount, sizeof(GLuint));
	if(!shaders) ERR_NOMEM_RET_ZERO;
	GLint success;
	for(int i=0; i<shaderCount; i++)
	{
		const char * currentFilename = va_arg(args, const char *);
		GLenum shaderType = va_arg(args,GLenum);
		shaders[i] = createShader(currentFilename,shaderType);
		glAttachShader(shaderProgram,shaders[i]);
	}
	glLinkProgram(shaderProgram);
	for(int i=0; i<shaderCount; i++)
	{
		glDeleteShader(shaders[i]);
	}
	free(shaders);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		char info_log[512]={0};
		glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
		setError(ERR_MESG,"Shader linking failure: %s",info_log);
		return 0;
	}
	return shaderProgram;
}

GLuint createShader(const char * shaderFilename, GLenum shaderType)
{
	const char * shaderSource = NULL;
	FILE * shaderFile = fopen(shaderFilename,"r");
	if(!shaderFile) ERR_NOFIL_RET_ZERO;
	fscanf(shaderFile,"%m[^\xff]",&shaderSource);
	if(!shaderSource) ERR_NOMEM_RET_ZERO;
	(void)fclose(shaderFile);
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID,1,&shaderSource,NULL);
	glCompileShader(shaderID);
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		char info_log[512]={0};
		glGetShaderInfoLog(shaderID, 512, NULL, info_log);
		setError(ERR_MESG,"Shader \"%s\" did not compile: %s\nFor reference, here is the source:\n=====\n%s\n=====",shaderFilename,info_log,shaderSource);
		//free(shaderSource);
		return 0;
	}
	//free(shaderSource);
	return shaderID;
}

GLuint createProgram(size_t shaderCount, ...)
{
	GLuint shaderProgram = glCreateProgram();
	va_list args;
	va_start(args, shaderCount);
	for(int i=0; i<shaderCount; i++)
	{
		GLuint currentShader = va_arg(args,GLuint);
		if(!currentShader) return 0;
		glAttachShader(shaderProgram,currentShader);
	}
	glLinkProgram(shaderProgram);
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		char info_log[512]={0};
		glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
		setError(ERR_MESG,"Shader linking failure: %s",info_log);
		return 0;
	}
	return shaderProgram;
}

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