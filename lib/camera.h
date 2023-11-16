#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
typedef uint32_t Uint32;

typedef struct camera camera_t;

/**
 * Creates a camera object, sets initial position, and hands it to the user 
 * 
 * @return A camera object with position {0,0,2} and yaw/pitch {0,0}
 */
camera_t *initCamera();

/**
 * Using the current window width and height, updates the camera's aspect ratio
 * 
 * @param cam the camera object to update
 * @returns the updated aspect ratio, or 0 if camera was NULL
 */
float updateCameraAspectRatio(camera_t *cam);

/**
 * According to the currently held buttons, moves the camera a set distance per call (using UPF == "Units per frame")
 * 
 * @param cam the camera object to move
 * @param buttonsHeld a bitfield of the currently held buttons
 * @return 0 on success, nonzero on fail (getError() for more details)
 */
int moveCamera(camera_t *cam, Uint32 buttonsHeld);

/**
 * 
 * 
 * @param cam 
 * @param modelMatrix 
 * @param mvpMatrix 
 */
int setMvpMatrix(camera_t *cam, mat4 modelMatrix, mat4 mvpMatrix);

#endif