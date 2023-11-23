#include "player.h"
#include "camera.h"
#include "world.h"

struct player
{
	camera_t *eyes;
	vec3 footPos;
	vec3 velocity;
};

player_t initPlayer(vec3 footPos)
{
	player_t player = calloc(1,sizeof(struct player));
	vec3 eyePos;
	glm_vec3_copy(footPos,eyePos);
	eyePos[Y] += PLAYER_EYE_HEIGHT;
	player->eyes = initCamera(eyePos);
	return player;
}

float playerUpdateCameraAspectRatio(player_t player)
{
	return updateCameraAspectRatio(player->eyes);
}

void playerRenderWorld(player_t player)
{
	renderWorld(player->eyes);
}

int movePlayer(player_t player, Uint32 buttonsHeld)
{
	return moveCamera(player->eyes,buttonsHeld);
}