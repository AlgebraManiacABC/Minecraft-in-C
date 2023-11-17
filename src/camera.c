#include "camera.h"
#include "debug.h"
#include "game.h"
#include "main.h"

struct camera
{
	vec3 pos;	// Unused
	float yaw;	// Yaw in radians (rotation about y axis - 0==2pi is facing -z)
	float pitch;// Pitch in radians (looking up/down - 0 is along xz-plane ("horizon"); -pi/2 <= pitch <= pi/2)
	float ar;	// Aspect Ratio
	vec3 dir;	// Camera direction as vec3
	float fovy;	// Vertical field of view
	float nearZ;
	float farZ;
	mat4 viewMatrix;	// The view Matrix (of MVP acclaim)
	mat4 projMatrix;	// The projection Matrix (of MVP acclaim)
};

camera_t *initCamera()
{
	camera_t *cam = malloc(sizeof(camera_t));
	if(!cam)
	{
		setError(ERR_CODE,ERR_NOMEM);
		return NULL;
	}
	cam->ar    = ASPECT_RATIO;
	cam->pitch = 0.0;
	cam->yaw   = 0.0;
	cam->pos[X]= 0.0;
	cam->pos[Y]= 0.0;
	cam->pos[Z]= 2.0;
	cam->dir[0] = cosf(cam->pitch) * sinf(cam->yaw + glm_rad(180));
	cam->dir[1] = sinf(cam->pitch);
	cam->dir[2] = cosf(cam->pitch) * cosf(cam->yaw + glm_rad(180));
	cam->fovy = glm_rad(90.0);
	cam->nearZ = 0.1f;
	cam->farZ = 100.0f;
	glm_look(cam->pos,cam->dir,worldUp,cam->viewMatrix);
	glm_perspective(cam->fovy,cam->ar,cam->nearZ,cam->farZ,cam->projMatrix);
	return cam;
}

float updateCameraAspectRatio(camera_t *cam)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return 0.0f;
	}
	cam->ar = ASPECT_RATIO;
	glm_perspective(cam->fovy,cam->ar,cam->nearZ,cam->farZ,cam->projMatrix);
	return ASPECT_RATIO;
}

void recalculateCameraDirection(camera_t *cam)
{
	if(!cam) return;
	cam->dir[0] = cosf(cam->pitch) * sinf(cam->yaw + glm_rad(180));
	cam->dir[1] = sinf(cam->pitch);
	cam->dir[2] = cosf(cam->pitch) * cosf(cam->yaw + glm_rad(180));
}

int moveCamera(camera_t *cam, Uint32 cameraBitfield)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return EXIT_FAILURE;
	}

	bool changedPitchYaw = false;

	if((cameraBitfield & CAMERA_MOVE_LEFT) && !(cameraBitfield & CAMERA_MOVE_RIGHT))
	{
		//	Move camera left
		cam->pos[X] -= (CAM_UPF)*sinf(cam->yaw + glm_rad(90));
		cam->pos[Z] -= (CAM_UPF)*cosf(cam->yaw + glm_rad(90));
	}
	else if((cameraBitfield & CAMERA_MOVE_RIGHT) && !(cameraBitfield & CAMERA_MOVE_LEFT))
	{
		//	Move camera right
		cam->pos[X] -= (CAM_UPF)*sinf(cam->yaw - glm_rad(90));
		cam->pos[Z] -= (CAM_UPF)*cosf(cam->yaw - glm_rad(90));
	}

	if((cameraBitfield & CAMERA_MOVE_FORWARD) && !(cameraBitfield & CAMERA_MOVE_BACKWARD))
	{
		//	Move camera forward
		cam->pos[X] -= (CAM_UPF)*sinf(cam->yaw);
		cam->pos[Z] -= (CAM_UPF)*cosf(cam->yaw);
	}
	else if((cameraBitfield & CAMERA_MOVE_BACKWARD) && !(cameraBitfield & CAMERA_MOVE_FORWARD))
	{
		//	Move camera backward
		cam->pos[X] += (CAM_UPF)*sinf(cam->yaw);
		cam->pos[Z] += (CAM_UPF)*cosf(cam->yaw);
	}

	if((cameraBitfield & CAMERA_MOVE_UP) && !(cameraBitfield & CAMERA_MOVE_DOWN))
	{
		//	Move camera up
		cam->pos[Y] += (CAM_UPF);
	}
	else if((cameraBitfield & CAMERA_MOVE_DOWN) && !(cameraBitfield & CAMERA_MOVE_UP))
	{
		//	Move camera down
		cam->pos[Y] -= (CAM_UPF);
	}

	if((cameraBitfield & CAMERA_YAW_LEFT) && !(cameraBitfield & CAMERA_YAW_RIGHT))
	{
		//	Turn camera left
		cam->yaw += (CAM_RPF);
		if(cam->yaw > glm_rad(360))
			cam->yaw -= glm_rad(360);
		changedPitchYaw = true;
	}
	else if((cameraBitfield & CAMERA_YAW_RIGHT) && !(cameraBitfield & CAMERA_YAW_LEFT))
	{
		//	Turn camera right
		cam->yaw -= (CAM_RPF);
		if(cam->yaw < 0)
			cam->yaw += glm_rad(360);
		changedPitchYaw = true;
	}

	if((cameraBitfield & CAMERA_PITCH_UP) && !(cameraBitfield & CAMERA_PITCH_DOWN))
	{
		//	Angle camera up
		cam->pitch += (CAM_RPF);
		if(cam->pitch >= glm_rad(90))
			cam->pitch = glm_rad(90 - 0.0625);
		changedPitchYaw = true;
	}
	else if((cameraBitfield & CAMERA_PITCH_DOWN) && !(cameraBitfield & CAMERA_PITCH_UP))
	{
		//	Angle camera down
		cam->pitch -= (CAM_RPF);
		if(cam->pitch <= glm_rad(-90))
			cam->pitch = glm_rad(-90 + 0.0625f);
		changedPitchYaw = true;
	}

	if(changedPitchYaw)
	{
		recalculateCameraDirection(cam);
	}
	glm_look(cam->pos,cam->dir,worldUp,cam->viewMatrix);
	return EXIT_SUCCESS;
}

int setMvpMatrix(camera_t *cam, mat4 modelMatrix, mat4 mvpMatrix)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return EXIT_FAILURE;
	}

	glm_mat4_mul(cam->projMatrix,cam->viewMatrix,mvpMatrix);
	glm_mat4_mul(mvpMatrix,modelMatrix,mvpMatrix);

	return EXIT_SUCCESS;
}

int setVpMatrix(camera_t *cam, mat4 vpMatrix)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return EXIT_FAILURE;
	}

	glm_mat4_mul(cam->projMatrix,cam->viewMatrix,vpMatrix);
	return EXIT_SUCCESS;
}