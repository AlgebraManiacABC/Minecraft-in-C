#ifndef DEBUG_H
#define DEBUG_H

#include "windows_compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
typedef uint32_t Uint32;

#define NODISCARD [[nodiscard]]

#define ERR_UNDEF (0)
#define ERR_NOFIL (1)
#define ERR_SHADE (2)
#define ERR_NULLP (3)
#define ERR_NOMEM (4)

typedef bool errType;
#define ERR_CODE (false)
#define ERR_MESG (true)

#define ERR_CODE_RETURN_X(code,retVal) { setError(ERR_CODE,code); return (retVal); }
#define ERR_CODE_RETURN(code) { setError(ERR_CODE,code); return; }
#define ERR_NULLP_RET_X(x) ERR_CODE_RETURN_X(ERR_NULLP,x)
#define ERR_NULLP_RET_NULL ERR_NULLP_RET_X(NULL)
#define ERR_NULLP_RET_FAIL ERR_NULLP_RET_X(EXIT_FAILURE)
#define ERR_NULLP_RET_ZERO ERR_NULLP_RET_X(0)
#define ERR_NULLP_RETURN ERR_CODE_RETURN(ERR_NULLP)
#define ERR_NOFIL_RET_X(x) ERR_CODE_RETURN_X(ERR_NOFIL,x)
#define ERR_NOFIL_RET_NULL ERR_NOFIL_RET_X(NULL)
#define ERR_NOFIL_RET_FAIL ERR_NOFIL_RET_X(EXIT_FAILURE)
#define ERR_NOFIL_RET_ZERO ERR_NOFIL_RET_X(0)
#define ERR_NOFIL_RETURN ERR_CODE_RETURN(ERR_NOFIL)
#define ERR_NOMEM_RET_X(x) ERR_CODE_RETURN_X(ERR_NOMEM,x)
#define ERR_NOMEM_RET_NULL ERR_NOMEM_RET_X(NULL)
#define ERR_NOMEM_RET_FAIL ERR_NOMEM_RET_X(EXIT_FAILURE)
#define ERR_NOMEM_RET_ZERO ERR_NOMEM_RET_X(0)
#define ERR_NOMEM_RETURN ERR_CODE_RETURN(ERR_NOMEM)

extern const char * errorMessages[];
extern const char * variableError;
extern char * editableError;
extern Uint32 lastErrorCode;

/**
 * Sets the error message to either a predefined error string (`ERR_CODE`) or a custom,
 * printf-style formatted string (`ERR_MESG`). The next argument is either the error code
 * or the formatted string (which would then be followed by any necessary matching variables)
 * 
 * @param type 
 * @param ... 
 */
void setError(errType type, ...);

NODISCARD
/**
 * Gets the most current error string set by setError().
 * Please use as soon as an error is detected,
 * otherwise the error message may get stale.
 * 
 * @returns the error string as a const char *
 */
const char * getError(void);

void print1dFloatArrayAsTable(float *arr, size_t rows, size_t cols);

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

#endif