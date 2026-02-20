#include <stdio.h>
#include "raylib.h"
#include "world.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Hello raylib!");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 8.0f, 17.0f, 8.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    DisableCursor();                // Disable cursor for camera movement

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    BlockWorld world;
    world.maxWidth = 16;
    world.maxHeight = 32;
    world.tempBlockLevel = 16;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        UpdateCameraPro(&camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*0.05f,                            // Rotation: yaw
                GetMouseDelta().y*0.05f,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            0.0f);                              // zoom disabled


        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawWorld(&world);

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    return 0;
}
