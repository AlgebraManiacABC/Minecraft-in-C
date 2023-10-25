#ifndef SHADERS_H
#define SHADERS_H

#define GLEW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <stdarg.h>
#include <stdlib.h>

GLuint createShader(const char * shaderFilename, GLenum shaderType);

GLuint createProgram(size_t shaderCount, ...);

#endif