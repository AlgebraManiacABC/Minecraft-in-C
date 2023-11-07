#include "camera.h"

#define PLAYER_HEIGHT (1.9)
#define PLAYER_EYE_HEIGHT (1.7)
#define PLAYER_WIDTH (0.75)

typedef struct player
{
	camera eyes;
	vec3 footPos;
	vec3 velocity;

}	player;