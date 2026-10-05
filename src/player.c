#include "../include/player.h"

#include <math.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

struct Player
{
    Vector3 pos;
    Vector3 vel;
    float h; // Player height
    float eyeh;
    float w; // Player width
    float yaw;
    float pitch;
    bool flying;
    float fov;
    BoundingBox boundingBox;
    Camera * camera;
};

typedef enum PlayerMovementDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
}   PlayerMovementDirection;

typedef struct PlayerMovement
{
    bool forward:1;
    bool backward:1;
    bool left:1;
    bool right:1;
    bool up:1;
    bool down:1;
}   PlayerMovement;

Vector3 PlayerGetVelocity(Player * player, PlayerMovement mov, float speed);
void PlayerMoveForward(Player * player, float distance);
void PlayerMoveRight(Player * player, float distance);
void PlayerMoveUp(Player * player, float distance);

static void UpdateBoundingBox(Player * player);
static void UpdateBoundingBoxX(Player * player);
static void UpdateBoundingBoxY(Player * player);
static void UpdateBoundingBoxZ(Player * player);

static void PlayerSetX(Player * player, float x);
static void PlayerAddX(Player * player, float dx);
static void PlayerSetY(Player * player, float y);
static void PlayerAddY(Player * player, float dy);
static void PlayerSetZ(Player * player, float z);
static void PlayerAddZ(Player * player, float dz);

static void PlayerSetYaw(Player * player, float yaw);
static void PlayerAddYaw(Player * player, float dYaw);
static void PlayerSetPitch(Player * player, float pitch);
static void PlayerAddPitch(Player * player, float dPitch);

Vector3 PlayerGetEyePosition(Player * player)
{
    return (Vector3){
            .x = player->pos.x,
            .y = player->pos.y + player->eyeh,
            .z = player->pos.z
    };
}

Camera * CreateCamera(Player * player)
{
    Camera * camera = calloc(1, sizeof(Camera));
    *camera = (Camera) {
        .position = PlayerGetEyePosition(player),
        .target = player->pos,
        .up = WORLD_UP,
        .fovy = player->fov,
        .projection = CAMERA_PERSPECTIVE
    };
    camera->target.z += 1; // Positive z is yaw == 0
    CameraPitch(camera, -player->pitch, true, false, false);
    CameraYaw(camera, -player->yaw, false);
    return camera;
}

Player * InitPlayer(Vector3 initPos, float initYaw, float initPitch, float initFov)
{
    Player * player = calloc(1, sizeof(Player));
    *player = (Player) {
        .h = 1.8f,
        .eyeh = 1.62f,
        .w = 0.6f,
        .fov = initFov,
        .pitch = initPitch,
        .yaw = initYaw
    };
    player->pos.x = initPos.x;
    player->pos.y = initPos.y;
    player->pos.z = initPos.z;
    UpdateBoundingBox(player);
    player->camera = CreateCamera(player);
    return player;
}

Camera * PlayerGetCamera(Player * player)
{
    return player->camera;
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
    PlayerMovement mov = {
        .forward = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP),
        .backward = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN),
        .left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT),
        .right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT),
        .up = IsKeyDown(KEY_SPACE),
        .down = IsKeyDown(KEY_LEFT_SHIFT)
    };

    float sensitivity = 0.05f;
    PlayerAddYaw(player, GetMouseDelta().x * sensitivity * DEG2RAD);
    PlayerAddPitch(player, GetMouseDelta().y * sensitivity * DEG2RAD);

    if (!mov.forward && !mov.backward && !mov.left && !mov.right && !mov.up && !mov.down) return;

    UpdateBoundingBox(player);

    BoundingBox worldBox = {
        .min = {0, 0, 0},
        .max = {world->maxWidth, world->tempBlockLevel, world->maxWidth}
    };

    Vector3 velocity = PlayerGetVelocity(player, mov, speed);
    Vector3 curPos = player->pos;

    PlayerSetX(player, player->pos.x + velocity.x);
    if (CheckCollisionBoxes(player->boundingBox, worldBox))
    {
        if (velocity.x > 0)
            PlayerSetX(player, worldBox.min.x - player->w / 2.0f - EPSILON);
        else
            PlayerSetX(player, worldBox.max.x + player->w / 2.0f + EPSILON);
    }

    PlayerSetZ(player, player->pos.z + velocity.z);
    if (CheckCollisionBoxes(player->boundingBox, worldBox))
    {
        if (velocity.z > 0)
            PlayerSetZ(player, worldBox.min.z - player->w / 2.0f - EPSILON);
        else
            PlayerSetZ(player, worldBox.max.z + player->w / 2.0f + EPSILON);
    }

    PlayerSetY(player, player->pos.y + velocity.y);
    if (CheckCollisionBoxes(player->boundingBox, worldBox))
    {
        if (velocity.y > 0)
            PlayerSetY(player, worldBox.min.y - player->h - EPSILON);
        else
            PlayerSetY(player, worldBox.max.y + EPSILON);
    }
}

Vector3 PlayerGetVelocity(Player * player, PlayerMovement mov, float speed)
{
    Vector3 velocity = {0};
    if (mov.forward)
    {
        velocity.x -= sinf(player->yaw) * speed;
        velocity.z += cosf(player->yaw) * speed;
    }
    else if (mov.backward)
    {
        velocity.x += sinf(player->yaw) * speed;
        velocity.z -= cosf(player->yaw) * speed;
    }

    if (mov.right)
    {
        velocity.x -= cosf(player->yaw) * speed;
        velocity.z -= sinf(player->yaw) * speed;
    }
    else if (mov.left)
    {
        velocity.x += cosf(player->yaw) * speed;
        velocity.z += sinf(player->yaw) * speed;
    }

    if (mov.up)
    {
        velocity.y += speed;
    }
    else if (mov.down)
    {
        velocity.y -= speed;
    }
    return velocity;
}

void PlayerMoveForward(Player * player, float distance)
{
    PlayerSetX(player, player->pos.x - sinf(player->yaw) * distance);
    PlayerSetZ(player, player->pos.z + cosf(player->yaw) * distance);
}

void PlayerMoveRight(Player *player, float distance)
{
    PlayerSetX(player, player->pos.x - cosf(player->yaw) * distance);
    PlayerSetZ(player, player->pos.z - sinf(player->yaw) * distance);
}

void PlayerMoveUp(Player *player, float distance)
{
    PlayerSetY(player, player->pos.y + distance);
}
static void PlayerSetX(Player * player, float x) {
    float dx = x - player->pos.x;
    PlayerAddX(player, dx);
}

static void PlayerAddX(Player * player, float dx)
{
    player->pos.x += dx;
    UpdateBoundingBoxX(player);
    player->camera->position.x += dx;
    player->camera->target.x += dx;
}

static void PlayerSetY(Player * player, float y) {
    player->pos.y = y;
    UpdateBoundingBoxY(player);
}

static void PlayerAddY(Player * player, float dy)
{
    player->pos.y += dy;
    UpdateBoundingBoxY(player);
    player->camera->position.y += dy;
    player->camera->target.y += dy;
}

static void PlayerSetZ(Player * player, float z) {
    player->pos.z = z;
    UpdateBoundingBoxZ(player);
}

static void PlayerAddZ(Player * player, float dz)
{
    player->pos.z += dz;
    UpdateBoundingBoxZ(player);
    player->camera->position.z += dz;
    player->camera->target.z += dz;
}

void UpdateBoundingBox(Player * player) {
    UpdateBoundingBoxX(player);
    UpdateBoundingBoxY(player);
    UpdateBoundingBoxZ(player);
}

static void UpdateBoundingBoxX(Player * player) {
    player->boundingBox.min.x = player->pos.x - player->w / 2.0f;
    player->boundingBox.max.x = player->pos.x + player->w / 2.0f;
}

static void UpdateBoundingBoxY(Player * player) {
    player->boundingBox.min.y = player->pos.y;
    player->boundingBox.max.y = player->pos.y + player->h;
}

static void UpdateBoundingBoxZ(Player * player) {
    player->boundingBox.min.z = player->pos.z - player->w / 2.0f;
    player->boundingBox.max.z = player->pos.z + player->w / 2.0f;
}

static void PlayerSetYaw(Player * player, float yaw)
{
    player->yaw = yaw;
    CameraYaw(player->camera, yaw - player->yaw, false);
}

static void PlayerAddYaw(Player * player, float dYaw)
{
    player->yaw += dYaw;
    CameraYaw(player->camera, -dYaw, false);
}

static void PlayerSetPitch(Player * player, float pitch)
{
    player->pitch = pitch;
    CameraPitch(player->camera, pitch - player->pitch, true, false, false);
}

static void PlayerAddPitch(Player * player, float dPitch)
{
    player->pitch += dPitch;
    CameraPitch(player->camera, -dPitch, true, false, false);
}
