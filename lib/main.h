#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>
#include <time.h>

#define INITIAL_WINDOW_W (800)
#define INITIAL_WINDOW_H (640)

extern SDL_Window *w;
extern int ww;
extern int wh;
#define ASPECT_RATIO (ww/(float)wh)