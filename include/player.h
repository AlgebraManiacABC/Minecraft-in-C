#ifndef PLAYER_H
#define PLAYER_H

#include "../include/world.h"

#include <raylib.h>

typedef struct Player Player;

Player * InitPlayer(Vector3 initPos, float initYaw, float initPitch, float initFov);

Camera GetPlayerCamera(Player *);

Vector3 PlayerGetPosition(Player *);

void UpdatePlayer(Player *, BlockWorld *);

#endif