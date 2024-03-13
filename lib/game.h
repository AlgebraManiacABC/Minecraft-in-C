#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>
#include <math.h>
#include <time.h>
#include "camera.h"
#include "player.h"

#define FPS (60.0f)
#define CAM_UPS (3.0f)	//	Units per second
#define CAM_RPS (1.0f)	//	Rotational units per second
#define CAM_UPF (CAM_UPS/FPS)	//	Units per frame
#define CAM_RPF (CAM_RPS/FPS)	//	Rotational units per frame

extern vec3 worldUp;

NODISCARD
/**
 * Main game loop.
 *
 */
Uint32 gameLoop();

#endif