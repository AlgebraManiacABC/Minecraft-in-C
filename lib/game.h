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

#define CAMERA_MOVE_LEFT        (0b00000000001)
#define CAMERA_MOVE_RIGHT       (0b00000000010)
#define CAMERA_MOVE_FORWARD     (0b00000000100)
#define CAMERA_MOVE_BACKWARD    (0b00000001000)
#define CAMERA_MOVE_UP          (0b00000010000)
#define CAMERA_MOVE_DOWN        (0b00000100000)
#define CAMERA_YAW_LEFT         (0b00001000000)
#define CAMERA_YAW_RIGHT        (0b00010000000)
#define CAMERA_PITCH_UP         (0b00100000000)
#define CAMERA_PITCH_DOWN       (0b01000000000)
#define SHADER_RELOAD_REQUESTED (0b10000000000)

#define FPS (60.0f)
#define CAM_UPS (3.0f)	//	Units per second
#define CAM_RPS (1.0f)	//	Rotational units per second
#define CAM_UPF (CAM_UPS/FPS)	//	Units per frame
#define CAM_RPF (CAM_RPS/FPS)	//	Rotational units per frame

extern vec3 worldUp;

/**
 * Main game loop.
 * 
 */
void gameLoop();

/**
 * Polls all incoming events through SDL
 * 
 * @param shouldClose Address to a bool which is set if SDL_QUIT (etc) is received
 * @param cam The camera_t struct, which will be updated depending on movement
 * @param buttonsHeld Uint32 bitfield of any pertinent buttons held (to be sent to moveCamera)
 * 
 * @returns The number of events polled, or (-1) if error
 */
int handleEvents(bool *shouldClose, camera_t *cam, Uint32 * buttonsHeld);

#endif