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

#define CAMERA_MOVE_LEFT     (0b0000000001)
#define CAMERA_MOVE_RIGHT    (0b0000000010)
#define CAMERA_MOVE_FORWARD  (0b0000000100)
#define CAMERA_MOVE_BACKWARD (0b0000001000)
#define CAMERA_MOVE_UP       (0b0000010000)
#define CAMERA_MOVE_DOWN     (0b0000100000)
#define CAMERA_YAW_LEFT      (0b0001000000)
#define CAMERA_YAW_RIGHT     (0b0010000000)
#define CAMERA_PITCH_UP      (0b0100000000)
#define CAMERA_PITCH_DOWN    (0b1000000000)

#define FPS (60.0f)
#define CAM_UPS (1.0f)	//	Units per second
#define CAM_UPF (CAM_UPS/FPS)	//	Units per frame

extern vec3 worldUp;

/**
 * Main game loop.
 * 
 * @param w The `SDL_Window *` created with a gl context
 */
void gameLoop(SDL_Window *w);

/**
 * Polls all incoming events through SDL
 * 
 * @param shouldClose Address to a bool which is set if SDL_QUIT (etc) is received
 * @param cam The camera struct, which will be updated depending on movement
 * @param buttonsHeld Uint32 bitfield of any pertinent buttons held (to be sent to moveCamera)
 * 
 * @returns The number of events polled 
 */
int handleEvents(bool *shouldClose, camera * cam, Uint32 * buttonsHeld);

#endif