#include "camera.h"
#include "debug.h"
#include "game.h"

camera initCamera(float aspectRatio)
{
	camera cam;
	cam.ar    = aspectRatio;
	cam.x     = 0.0;
	cam.y     = 0.0;
	cam.z     = 2.0;
	cam.pitch = 0.0;
	cam.yaw   = 0.0;
	cam.pos[0]= 0.0;
	cam.pos[1]= 0.0;
	cam.pos[2]= 2.0;
	return cam;
}

int updateCameraAspectRatio(camera * cam, float aspectRatio)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return EXIT_FAILURE;
	}
	cam->ar = aspectRatio;
	return EXIT_SUCCESS;
}

int moveCamera(camera * cam, Uint32 cameraBitfield)
{
	if(!cam)
	{
		setError(ERR_CODE,ERR_NULLP);
		return EXIT_FAILURE;
	}

	if((cameraBitfield & CAMERA_MOVE_LEFT) && !(cameraBitfield & CAMERA_MOVE_RIGHT))
	{
		//	Move camera left
		cam->x -= (CAM_UPF)*sinf(cam->yaw + glm_rad(90));
		cam->z -= (CAM_UPF)*cosf(cam->yaw + glm_rad(90));
	}
	else if((cameraBitfield & CAMERA_MOVE_RIGHT) && !(cameraBitfield & CAMERA_MOVE_LEFT))
	{
		//	Move camera right
		cam->x -= (CAM_UPF)*sinf(cam->yaw - glm_rad(90));
		cam->z -= (CAM_UPF)*cosf(cam->yaw - glm_rad(90));
	}

	if((cameraBitfield & CAMERA_MOVE_FORWARD) && !(cameraBitfield & CAMERA_MOVE_BACKWARD))
	{
		//	Move camera forward
		cam->x -= (CAM_UPF)*sinf(cam->yaw);
		cam->z -= (CAM_UPF)*cosf(cam->yaw);
	}
	else if((cameraBitfield & CAMERA_MOVE_BACKWARD) && !(cameraBitfield & CAMERA_MOVE_FORWARD))
	{
		//	Move camera backward
		cam->x += (CAM_UPF)*sinf(cam->yaw);
		cam->z += (CAM_UPF)*cosf(cam->yaw);
	}

	if((cameraBitfield & CAMERA_MOVE_UP) && !(cameraBitfield & CAMERA_MOVE_DOWN))
	{
		//	Move camera up
		cam->y += (CAM_UPF);
	}
	else if((cameraBitfield & CAMERA_MOVE_DOWN) && !(cameraBitfield & CAMERA_MOVE_UP))
	{
		//	Move camera down
		cam->y -= (CAM_UPF);
	}

	if((cameraBitfield & CAMERA_YAW_LEFT) && !(cameraBitfield & CAMERA_YAW_RIGHT))
	{
		//	Turn camera left
		cam->yaw += (CAM_UPF);
		if(cam->yaw > glm_rad(360))
			cam->yaw -= glm_rad(360);
	}
	else if((cameraBitfield & CAMERA_YAW_RIGHT) && !(cameraBitfield & CAMERA_YAW_LEFT))
	{
		//	Turn camera right
		cam->yaw -= (CAM_UPF);
		if(cam->yaw < 0)
			cam->yaw += glm_rad(360);
	}

	if((cameraBitfield & CAMERA_PITCH_UP) && !(cameraBitfield & CAMERA_PITCH_DOWN))
	{
		//	Angle camera up
		cam->pitch += (CAM_UPF);
		if(cam->pitch >= glm_rad(90))
			cam->pitch = glm_rad(90 - 0.0625);
	}
	else if((cameraBitfield & CAMERA_PITCH_DOWN) && !(cameraBitfield & CAMERA_PITCH_UP))
	{
		//	Angle camera down
		cam->pitch -= (CAM_UPF);
		if(cam->pitch <= glm_rad(-90))
			cam->pitch = glm_rad(-90 + 0.0625f);
	}

	return EXIT_SUCCESS;
}

void setMvpMatrix(camera cam, mat4 modelMatrix, mat4 mvpMatrix)
{
	vec3 cameraDirection =
	{
		cosf(cam.pitch) * sinf(cam.yaw + glm_rad(180)),
		sinf(cam.pitch),
		cosf(cam.pitch) * cosf(cam.yaw + glm_rad(180))
	};

	mat4 viewMatrix;
	glm_look((vec3){cam.x,cam.y,cam.z},cameraDirection,(vec3){0,1,0},viewMatrix);
	mat4 projectionMatrix;
	float fovy = glm_rad(90);
	float nearZ = 0.1f;
	float farZ = 100.0f;
	glm_perspective(fovy,cam.ar,nearZ,farZ,projectionMatrix);

	glm_mat4_mul(projectionMatrix,viewMatrix,mvpMatrix);
	glm_mat4_mul(mvpMatrix,modelMatrix,mvpMatrix);
}