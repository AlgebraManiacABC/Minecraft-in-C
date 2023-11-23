#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include "debug.h"
typedef uint32_t Uint32;

#define X 0
#define Y 1
#define Z 2

typedef struct camera camera_t;

NODISCARD
/**
 * Creates a camera object, sets initial position and direction, and hands it to the user 
 * 
 * @param pos the position to place the camera
 * @return A camera object with position {0,0,2} and yaw/pitch {0,0}
 */
camera_t *initCamera(vec3 pos);

NODISCARD
/**
 * Using the current window width and height, updates the camera's aspect ratio
 * 
 * @param cam the camera object to update
 * @returns the updated aspect ratio, or 0 if camera was NULL
 */
float updateCameraAspectRatio(camera_t *cam);

NODISCARD
/**
 * According to the currently held buttons, moves the camera a set distance per call (using UPF == "Units per frame")
 * 
 * @param cam the camera object to move
 * @param buttonsHeld a bitfield of the currently held buttons
 * @return 0 on success, nonzero on fail (getError() for more details)
 */
int moveCamera(camera_t *cam, Uint32 buttonsHeld);

NODISCARD
/**
 * Sets the modelViewProjection Matrix according to the camera and model matrix
 * 
 * @param cam 
 * @param modelMatrix 
 * @param mvpMatrix 
 * @return `EXIT_SUCCESS` on success, `EXIT_FAILURE` on failure
 */
int setMvpMatrix(camera_t *cam, mat4 modelMatrix, mat4 mvpMatrix);

NODISCARD
/**
 * Sets the viewProjection / projectionView matrix according to the camera's information
 * 
 * @param cam 
 * @param vpMatrix 
 * @return `EXIT_SUCCESS` on success, `EXIT_FAILURE` on failure
 */
int setVpMatrix(camera_t *cam, mat4 vpMatrix);

#endif