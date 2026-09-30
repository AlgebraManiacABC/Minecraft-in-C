#include "../include/player.h"

#include <math.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
static void PlayerSetY(Player * player, float y);
static void PlayerSetZ(Player * player, float z);

Player * InitPlayer(Vector3 initPos, float initYaw, float initPitch, float initFov)
{
    Player * player = calloc(1, sizeof(Player));
    player->h = 1.8f;
    player->eyeh = 1.62f;
    player->w = 0.6f;
    player->fov = initFov;
    player->pitch = initPitch;
    player->yaw = initYaw;
    PlayerSetX(player, initPos.x);
    PlayerSetY(player, initPos.y);
    PlayerSetZ(player, initPos.z);
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
    PlayerMovement mov = {
        .forward = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP),
        .backward = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN),
        .left = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT),
        .right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT),
        .up = IsKeyDown(KEY_SPACE),
        .down = IsKeyDown(KEY_LEFT_SHIFT)
    };

    float sensitivity = 0.05f;
    player->yaw += GetMouseDelta().x * sensitivity * DEG2RAD;
    player->pitch += GetMouseDelta().y * sensitivity * DEG2RAD;

    if (!mov.forward && !mov.backward && !mov.left && !mov.right && !mov.up && !mov.down) return;

    UpdateBoundingBox(player);

    BoundingBox worldBox = {
        .min = {0, 0, 0},
        .max = {world->maxWidth, world->tempBlockLevel, world->maxWidth}
    };

    Vector3 velocity = PlayerGetVelocity(player, mov, speed);
    Vector3 posBeforeCollision = player->pos;

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
        velocity.x -= -(sinf(player->yaw) * speed);
        velocity.z += -(cosf(player->yaw) * speed);
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
    // player->pos.x -= sinf(player->yaw) * distance;
    // player->pos.z += cosf(player->yaw) * distance;
}

void PlayerMoveRight(Player *player, float distance)
{
    PlayerSetX(player, player->pos.x - cosf(player->yaw) * distance);
    PlayerSetZ(player, player->pos.z - sinf(player->yaw) * distance);
    // player->pos.x -= cosf(player->yaw) * distance;
    // player->pos.z -= sinf(player->yaw) * distance;
}

void PlayerMoveUp(Player *player, float distance)
{
    PlayerSetY(player, player->pos.y + distance);
    // player->pos.y += distance;
}
static void PlayerSetX(Player * player, float x) {
    player->pos.x = x;
    UpdateBoundingBoxX(player);
}

static void PlayerSetY(Player * player, float y) {
    player->pos.y = y;
    UpdateBoundingBoxY(player);
}

static void PlayerSetZ(Player * player, float z) {
    player->pos.z = z;
    UpdateBoundingBoxZ(player);
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