#include "camera.h"

#define PLAYER_HEIGHT (1.9)
#define PLAYER_EYE_HEIGHT (1.7)
#define PLAYER_WIDTH (0.75)

#define PLAYER_DEFAULT_SPEED (4.5)
#define PLAYER_SPRINT_SPEED (6.0)

typedef struct player
{
	camera_t *eyes;
	vec3 footPos;
	vec3 velocity;

}	player;