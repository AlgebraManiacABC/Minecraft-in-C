#include "player.h"
#include "camera.h"
#include "world.h"
#include "events.h"
#include "game.h"
#include "main.h"

#define INITIAL_SENSITIVITY (0.01f)

typedef unsigned int uint;

struct player
{
	camera_t *eyes;
	vec3 footPos;
	vec3 velocity;
	// Defined as a percent of a full 2pi radians (360deg) to turn for every pixel the mouse moved.
	float sensitivity;
};

player_t initPlayer(vec3 footPos)
{
	player_t player = calloc(1,sizeof(struct player));
	vec3 eyePos;
	glm_vec3_copy(footPos,eyePos);
	eyePos[Y] += PLAYER_EYE_HEIGHT;
	player->eyes = initCamera(eyePos);
	player->sensitivity = INITIAL_SENSITIVITY;
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
	if(!player) ERR_NULLP_RET_FAIL;

	if((buttonsHeld & CAMERA_MOVE_LEFT) && !(buttonsHeld & CAMERA_MOVE_RIGHT))
	{
		//	Move player left
		if(relativeTranslateCamera(player->eyes,VEC3(-CAM_UPF,0,0))) return EXIT_FAILURE;
	}
	else if((buttonsHeld & CAMERA_MOVE_RIGHT) && !(buttonsHeld & CAMERA_MOVE_LEFT))
	{
		//	Move player right
		if(relativeTranslateCamera(player->eyes,VEC3(CAM_UPF,0,0))) return EXIT_FAILURE;
	}

	if((buttonsHeld & CAMERA_MOVE_FORWARD) && !(buttonsHeld & CAMERA_MOVE_BACKWARD))
	{
		//	Move player forward
		if(relativeTranslateCamera(player->eyes,VEC3(0,0,-CAM_UPF))) return EXIT_FAILURE;
	}
	else if((buttonsHeld & CAMERA_MOVE_BACKWARD) && !(buttonsHeld & CAMERA_MOVE_FORWARD))
	{
		//	Move player backward
		if(relativeTranslateCamera(player->eyes,VEC3(0,0,CAM_UPF))) return EXIT_FAILURE;
	}

	if((buttonsHeld & CAMERA_MOVE_UP) && !(buttonsHeld & CAMERA_MOVE_DOWN))
	{
		//	Move player up
		if(relativeTranslateCamera(player->eyes,VEC3(0,CAM_UPF,0))) return EXIT_FAILURE;
	}
	else if((buttonsHeld & CAMERA_MOVE_DOWN) && !(buttonsHeld & CAMERA_MOVE_UP))
	{
		//	Move player down
		if(relativeTranslateCamera(player->eyes,VEC3(0,-CAM_UPF,0))) return EXIT_FAILURE;
	}

	//if(changeCameraYaw(player->eyes,(player->sensitivity * mx))) return EXIT_FAILURE;
	if((buttonsHeld & CAMERA_YAW_LEFT) && !(buttonsHeld & CAMERA_YAW_RIGHT))
	{
		//	Turn player left
		if(changeCameraYaw(player->eyes,player->sensitivity)) return EXIT_FAILURE;
	}
	else if((buttonsHeld & CAMERA_YAW_RIGHT) && !(buttonsHeld & CAMERA_YAW_LEFT))
	{
		//	Turn player right
		if(changeCameraYaw(player->eyes,-player->sensitivity)) return EXIT_FAILURE;
	}

	//if(changeCameraPitch(player->eyes,(player->sensitivity * my))) return EXIT_FAILURE;
	if((buttonsHeld & CAMERA_PITCH_UP) && !(buttonsHeld & CAMERA_PITCH_DOWN))
	{
		//	Angle player up
		if(changeCameraPitch(player->eyes,player->sensitivity)) return EXIT_FAILURE;
	}
	else if((buttonsHeld & CAMERA_PITCH_DOWN) && !(buttonsHeld & CAMERA_PITCH_UP))
	{
		//	Angle player down
		if(changeCameraPitch(player->eyes,-player->sensitivity)) return EXIT_FAILURE;
	}

	recalculateCameraViewMatrix(player->eyes);
	return EXIT_SUCCESS;
}