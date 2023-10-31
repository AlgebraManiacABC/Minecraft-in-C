#include "shaders.h"
#include "debug.h"

GLuint createShader(const char * shaderFilename, GLenum shaderType)
{
	const char * shaderSource = NULL;
	FILE * shaderFile = fopen(shaderFilename,"r");
	if(!shaderFile)
	{
		setError(ERR_CODE,ERR_NOFIL);
		return 0;
	}
	fscanf(shaderFile,"%m[^\xff]",&shaderSource);
	if(!shaderSource)
	{
		setError(ERR_CODE,ERR_NOMEM);
		return 0;
	}
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
		setError(ERR_MESG,"Shader did not compile: %s\nFor reference, here is the source:\n```\n%s\n```",info_log,shaderSource);
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