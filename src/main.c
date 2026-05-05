#include <stdio.h>

#include "player.h"
#include "raylib.h"
#include "../include/world.h"

void RenderMain(Player * player, BlockWorld * world, RenderTexture texture);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Hello raylib!");

    DisableCursor();                // Disable cursor for camera movement

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Player * player = InitPlayer((Vector3){-10.0f, 17.0f, -10.0f}, 0, 0, 45.0f);

    BlockWorld * world = InitWorld(16, 32, 16);
    UpdateWorldMesh(world);

    RenderTexture blockTextures = LoadRenderTexture(1, 1);
    BeginTextureMode(blockTextures);
    ClearBackground(BLANK);
    DrawRectangle(0, 0, 1, 1, DARKGREEN);
    EndTextureMode();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdatePlayer(player, world);
        RenderMain(player, world, blockTextures);
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return 0;
}

void RenderMain(Player * player, BlockWorld * world, RenderTexture texture)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(GetPlayerCamera(player));

    DrawWorld(world, texture);

    EndMode3D();

    DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

    DrawFPS(10, 10);

    char posStr[] = "Player position: ______, ______, ______";
    Vector3 pos = PlayerGetPosition(player);
    sprintf(posStr, "Player position: %03.02f, %03.02f, %03.02f", pos.x, pos.y, pos.z);
    DrawText(posStr, 10, 60, 20, DARKBLUE);

    EndDrawing();
}
