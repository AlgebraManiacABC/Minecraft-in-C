#include "../include/player.h"

#include <math.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Player
{
    Vector3 pos;
    Vector3 vel;
    float yaw;
    float pitch;
    bool flying;
    float fov;
};

void PlayerMoveForward(Player * player, float distance);
void PlayerMoveRight(Player * player, float distance);
void PlayerMoveUp(Player * player, float distance);

Player * InitPlayer(Vector3 initPos, float initYaw, float initPitch, float initFov)
{
    Player * player = calloc(1, sizeof(Player));
    player->pos.x = initPos.x;
    player->pos.y = initPos.y;
    player->pos.z = initPos.z;
    player->fov = initFov;
    player->pitch = initPitch;
    player->yaw = initYaw;
    return player;
}

Camera CreateCamera(Player * player)
{
    Camera camera = {
        .position = player->pos,
        .target = player->pos,
        .up = WORLD_UP,
        .fovy = player->fov,
        .projection = CAMERA_PERSPECTIVE
    };
    camera.target.z += 1; // Positive z is yaw == 0
    CameraPitch(&camera, -player->pitch, true, false, false);
    CameraYaw(&camera, -player->yaw, false);
    return camera;
}

Camera GetPlayerCamera(Player * player)
{
    return CreateCamera(player);
}

Vector3 PlayerGetPosition(Player * player)
{
    return player->pos;
}

Ray PlayerCreateMovementRay(Player * player, float forward, float up, float right)
{
    float x = forward * -sinf(player->yaw) + right * -cosf(player->yaw);
    float y = up;
    float z = forward * cosf(player->yaw) + right * -sinf(player->yaw);
    return (Ray){
        player->pos,
        Vector3Normalize((Vector3){x, y, z})
    };
}

void UpdatePlayer(Player * player, BlockWorld * world)
{
    float speed = 0.1f;
    bool forward = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool backward = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    bool up = IsKeyDown(KEY_SPACE);
    bool down = IsKeyDown(KEY_LEFT_SHIFT);

    float sensitivity = 0.05f;
    player->yaw += GetMouseDelta().x * sensitivity * DEG2RAD;
    player->pitch += GetMouseDelta().y * sensitivity * DEG2RAD;

    if (!forward && !backward && !left && !right && !up && !down) return;

    // PlayerMoveForward(player, forward * speed - backward * speed);
    // PlayerMoveRight(player, right * speed - left * speed);
    // PlayerMoveUp(player, up * speed - down * speed);
    Ray toMove = PlayerCreateMovementRay(player,
        forward - backward,
        up - down,
        right - left
    );
    RayCollision collide = GetRayCollisionMesh(toMove, *world->mesh, MatrixIdentity());
    Vector3 dir = toMove.direction;
    dir = Vector3Scale(dir, speed);
    if (collide.hit && collide.distance <= Vector3Length(dir))
    {
        dir = Vector3Scale(dir, (collide.distance / Vector3Length(dir)) - 0.01f);
        player->pos = Vector3Add(player->pos, dir);
    }
    else
    {
        player->pos = Vector3Add(player->pos, dir);
    }
}

void PlayerMoveForward(Player * player, float distance)
{
    player->pos.x -= sin(player->yaw) * distance;
    player->pos.z += cos(player->yaw) * distance;
}

void PlayerMoveRight(Player *player, float distance)
{
    player->pos.x -= cos(player->yaw) * distance;
    player->pos.z -= sin(player->yaw) * distance;
}

void PlayerMoveUp(Player *player, float distance)
{
    player->pos.y += distance;
}
