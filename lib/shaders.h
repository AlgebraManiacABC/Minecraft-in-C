#ifndef SHADERS_H
#define SHADERS_H

#define GLEW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <stdarg.h>

GLuint mcCreateShader(const char * shaderFilename, GLenum shaderType);

GLuint mcCreateProgram(size_t shaderCount, ...);

#endif