#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H
#ifdef _WIN32

/**
 * @file windows_compat.h
 * This file renames the mingw functions for (v)asprintf to be used as they should be named.
 * It also defines _GNU_SOURCE so these functions (among other useful features) can be used in linux.
 * 
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int asprintf(char ** str, const char * fmt, ...);
int vasprintf(char ** str, const char * fmt, va_list args);

#else
	#define _GNU_SOURCE
#endif
#endif