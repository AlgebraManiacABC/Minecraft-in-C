#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
typedef uint32_t Uint32;

typedef struct camera camera_t;

camera_t *initCamera();

int updateCameraAspectRatio(camera_t *cam);

int moveCamera(camera_t *cam, Uint32 buttonsHeld);

void setMvpMatrix(camera_t *cam, mat4 modelMatrix, mat4 mvpMatrix);

#endif