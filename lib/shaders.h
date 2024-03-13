#ifndef SHADERS_H
#define SHADERS_H

#define GLEW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.h"

extern GLint vpMatLocus;
extern GLint mMatLocus;

NODISCARD
/**
 * Given a number of shader source files and their types, links them to a shader program
 *
 * @param shaderCount the number of shaders to link
 * @param ... `shaderCount` pairs of [const char * shaderFilename, GLenum shaderType] used to create each shader
 * @return a GLuint handle to the shader program, or 0 on failure.
 */
GLuint createShaderProgram(size_t shaderCount, ...);

NODISCARD
/**
 * From a shader file, compiles the shader and returns a GLuint
 *
 * @param shaderFilename the filename of the shader
 * @param shaderType `GL_VERTEX_SHADER` or `GL_FRAGMENT_SHADER`
 * @return a GLuint handle to the shader, or 0 on failure.
 */
GLuint createShader(const char * shaderFilename, GLenum shaderType);

NODISCARD
/**
 * Given a number of shaders, links them into a shader program
 *
 * @param shaderCount the number of shaders to link
 * @param ... The GLuint handles to each shader to link
 * @return a GLuint handle to the shader program, or 0 on failure.
 */
GLuint createProgram(size_t shaderCount, ...);

NODISCARD
/**
 * Destroys the shaders and recompiles them into a new program, which is applied.
 *
 * @return the shader GLuint if successful, 0 on fail
 */
GLuint reloadShaders();

#endif