#include "player.h"
#include "camera.h"
#include "world.h"
#include "events.h"
#include "game.h"
#include "main.h"

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
	//return moveCamera(player->eyes,buttonsHeld);
	if(!player) ERR_NULLP_RET_FAIL;

	if((buttonsHeld & CAMERA_MOVE_LEFT) && !(buttonsHeld & CAMERA_MOVE_RIGHT))
	{
		//	Move camera left
		relativeTranslateCamera(player->eyes,VEC3(-CAM_UPF,0,0));
	}
	else if((buttonsHeld & CAMERA_MOVE_RIGHT) && !(buttonsHeld & CAMERA_MOVE_LEFT))
	{
		//	Move camera right
		relativeTranslateCamera(player->eyes,VEC3(CAM_UPF,0,0));
	}

	if((buttonsHeld & CAMERA_MOVE_FORWARD) && !(buttonsHeld & CAMERA_MOVE_BACKWARD))
	{
		//	Move camera forward
		relativeTranslateCamera(player->eyes,VEC3(0,0,-CAM_UPF));
	}
	else if((buttonsHeld & CAMERA_MOVE_BACKWARD) && !(buttonsHeld & CAMERA_MOVE_FORWARD))
	{
		//	Move camera backward
		relativeTranslateCamera(player->eyes,VEC3(0,0,CAM_UPF));
	}

	if((buttonsHeld & CAMERA_MOVE_UP) && !(buttonsHeld & CAMERA_MOVE_DOWN))
	{
		//	Move camera up
		relativeTranslateCamera(player->eyes,VEC3(0,CAM_UPF,0));
	}
	else if((buttonsHeld & CAMERA_MOVE_DOWN) && !(buttonsHeld & CAMERA_MOVE_UP))
	{
		//	Move camera down
		relativeTranslateCamera(player->eyes,VEC3(0,-CAM_UPF,0));
	}

	if((buttonsHeld & CAMERA_YAW_LEFT) && !(buttonsHeld & CAMERA_YAW_RIGHT))
	{
		//	Turn camera left
		changeCameraYaw(player->eyes,CAM_RPF);
	}
	else if((buttonsHeld & CAMERA_YAW_RIGHT) && !(buttonsHeld & CAMERA_YAW_LEFT))
	{
		//	Turn camera right
		changeCameraYaw(player->eyes,-CAM_RPF);
	}

	if((buttonsHeld & CAMERA_PITCH_UP) && !(buttonsHeld & CAMERA_PITCH_DOWN))
	{
		//	Angle camera up
		changeCameraPitch(player->eyes,CAM_RPF);
	}
	else if((buttonsHeld & CAMERA_PITCH_DOWN) && !(buttonsHeld & CAMERA_PITCH_UP))
	{
		//	Angle camera down
		changeCameraPitch(player->eyes,-CAM_RPF);
	}

	recalculateCameraViewMatrix(player->eyes);
	return EXIT_SUCCESS;
}