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

#define VEC3(x,y,z) ((vec3){x,y,z})

#define INITIAL_WINDOW_W (800)
#define INITIAL_WINDOW_H (640)

//	The global window object
extern SDL_Window *w;
//	The global window width
extern int ww;
//	The global window height
extern int wh;
//	The aspect ratio of the window, as determined by the global window dimensions
#define ASPECT_RATIO (ww/(float)wh)

//	The global mouse x position
extern int mx;
//	The global mouse y position
extern int my;