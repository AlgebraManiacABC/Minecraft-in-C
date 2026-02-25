#include "world.h"
#include "raylib.h"
#include <math.h>

enum blocks {
    air = 0,
    dirt = 1,
    stone = 2,
    wood = 3,
    grass = 4,
    sand = 5,
    gravel = 6,
    cobblestone = 7
};

// Block structure with velocity
typedef struct {
    int type;
    Vector3 position;
    Vector3 velocity;
    float gravity;
} Block;

// Player structure
typedef struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float gravity;
    float jumpForce;
    float moveSpeed;
    float radius;
    int isGrounded;
} Player;

// Get color based on block type
Color GetBlockColor(int blockType)
{
    switch(blockType)
    {
        case dirt:
            return (Color){139, 90, 43, 255};      // Brown
        case stone:
            return (Color){128, 128, 128, 255};    // Gray
        case wood:
            return (Color){101, 67, 33, 255};      // Dark brown
        case grass:
            return (Color){34, 139, 34, 255};      // Forest green
        case sand:
            return (Color){238, 214, 175, 255};    // Sandy
        case gravel:
            return (Color){169, 169, 169, 255};    // Dark gray
        case cobblestone:
            return (Color){105, 105, 105, 255};    // Dim gray
        case air:
        default:
            return BLANK;
    }
}

// Check if a position contains a solid block
int IsBlockSolid(BlockWorld * world, Vector3 pos)
{
    // Bounds checking
    if (pos.x < 0 || pos.x >= world->maxWidth ||
        pos.z < 0 || pos.z >= world->maxWidth ||
        pos.y < 0 || pos.y >= world->tempBlockLevel)
    {
        return 0;
    }
    
    int x = (int)pos.x;
    int z = (int)pos.z;
    int y = (int)pos.y;
    
    // Access your block data
    return world->blocks[x][z][y] != air;
}

// Check AABB collision between entity and world blocks
int CheckBlockCollision(BlockWorld * world, Vector3 entityPos, float radius)
{
    // Check surrounding blocks within collision radius
    int minX = (int)(entityPos.x - radius);
    int maxX = (int)(entityPos.x + radius);
    int minY = (int)(entityPos.y - radius);
    int maxY = (int)(entityPos.y + radius);
    int minZ = (int)(entityPos.z - radius);
    int maxZ = (int)(entityPos.z + radius);
    
    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            for (int z = minZ; z <= maxZ; z++)
            {
                Vector3 worldBlockPos = {(float)x, (float)y, (float)z};
                if (IsBlockSolid(world, worldBlockPos))
                {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

// Check if player is grounded
int IsPlayerGrounded(Player * player, BlockWorld * world)
{
    Vector3 belowPos = Vector3Add(player->position, (Vector3){0, (float)(-0.1), 0});
    return CheckBlockCollision(world, belowPos, player->radius);
}

// Initialize player
void InitPlayer(Player * player, Vector3 startPos)
{
    player->position = startPos;
    player->velocity = (Vector3){0, 0, 0};
    player->acceleration = (Vector3){0, 0, 0};
    player->gravity = (float)25;           // Gravity strength
    player->jumpForce = (float)12;         // Jump velocity
    player->moveSpeed = (float)8;          // Movement speed
    player->radius = (float)4;             // Collision radius
    player->isGrounded = 0;
}

// Handle player input
void HandlePlayerInput(Player * player)
{
    Vector3 moveDirection = {0, 0, 0};
    
    // WASD movement
    if (IsKeyDown(KEY_W))
        moveDirection.z += 1;
    if (IsKeyDown(KEY_S))
        moveDirection.z -= 1;
    if (IsKeyDown(KEY_A))
        moveDirection.x -= 1;
    if (IsKeyDown(KEY_D))
        moveDirection.x += 1;
    
    // Normalize diagonal movement
    if (moveDirection.x != 0 || moveDirection.z != 0)
    {
        float length = sqrtf(moveDirection.x * moveDirection.x + moveDirection.z * moveDirection.z);
        moveDirection.x /= length;
        moveDirection.z /= length;
    }
    
    // Apply movement
    player->velocity.x = moveDirection.x * player->moveSpeed;
    player->velocity.z = moveDirection.z * player->moveSpeed;
    
    // Jump
    if (IsKeyPressed(KEY_SPACE) && player->isGrounded)
    {
        player->velocity.y = player->jumpForce;
        player->isGrounded = 0;
    }
}

// Update player physics
void UpdatePlayer(Player * player, BlockWorld * world, float deltaTime)
{
    // Check if grounded before applying gravity
    player->isGrounded = IsPlayerGrounded(player, world);
    
    // Apply gravity
    if (!player->isGrounded)
    {
        player->velocity.y -= player->gravity * deltaTime;
    }
    else
    {
        // Stop downward velocity when grounded
        if (player->velocity.y < 0)
            player->velocity.y = 0;
    }
    
    // Calculate new position
    Vector3 newPos = Vector3Add(player->position, Vector3Scale(player->velocity, deltaTime));
    
    // Check collision
    if (!CheckBlockCollision(world, newPos, player->radius))
    {
        player->position = newPos;
    }
    else
    {
        // Handle collision - stop movement in colliding axis
        Vector3 posX = Vector3Add(player->position, (Vector3){player->velocity.x * deltaTime, 0, 0});
        Vector3 posZ = Vector3Add(player->position, (Vector3){0, 0, player->velocity.z * deltaTime});
        Vector3 posY = Vector3Add(player->position, (Vector3){0, player->velocity.y * deltaTime, 0});
        
        if (!CheckBlockCollision(world, posX, player->radius))
            player->position.x = posX.x;
        if (!CheckBlockCollision(world, posZ, player->radius))
            player->position.z = posZ.z;
        if (!CheckBlockCollision(world, posY, player->radius))
            player->position.y = posY.y;
        else
            player->velocity.y = 0;
    }
}

// Draw player
void DrawPlayer(Player * player)
{
    DrawCube(player->position, (float)0.6, (float)1.8, (float)0.6, BLUE);
    DrawCubeWires(player->position, (float)0.6, (float)1.8, (float)0.6, DARKBLUE);
}

// Update falling block physics
void UpdateBlock(Block * block, BlockWorld * world, float deltaTime)
{
    // Apply gravity
    block->velocity.y -= block->gravity * deltaTime;
    
    // Update position
    Vector3 newPos = Vector3Add(block->position, Vector3Scale(block->velocity, deltaTime));
    
    // Check collision
    if (!CheckBlockCollision(world, newPos, (float)0.4))
    {
        block->position = newPos;
    }
    else
    {
        // Stop falling on collision
        block->velocity.y = 0;
        block->position.y = (float)((int)block->position.y + 1);
    }
}

// Draw a single falling block
void DrawBlock(Block * block)
{
    if (block->type != air)
    {
        Color blockColor = GetBlockColor(block->type);
        DrawCube(block->position, 1, 1, 1, blockColor);
        DrawCubeWires(block->position, 1, 1, 1, BLACK);
    }
}

void DrawWorld(BlockWorld * world)
{
    // Start drawing the world at 0, 0, 0
    Vector3 tempPos = {0, 0, 0};
    for (tempPos.x = 0; tempPos.x < world->maxWidth; tempPos.x++)
    {
        for (tempPos.z = 0; tempPos.z < world->maxWidth; tempPos.z++)
        {
            for (tempPos.y = 0; tempPos.y < world->tempBlockLevel; tempPos.y++)
            {
                // Get block type from world data
                int blockType = world->blocks[(int)tempPos.x][(int)tempPos.z][(int)tempPos.y];
                
                // Only draw solid blocks
                if (blockType != air)
                {
                    Color blockColor = GetBlockColor(blockType);
                    DrawCube(tempPos, 1, 1, 1, blockColor);
                    DrawCubeWires(tempPos, 1, 1, 1, BLACK);
                }
            }
        }
    }
}

// Draw all falling blocks
void DrawFallingBlocks(Block * blocks, int blockCount)
{
    for (int i = 0; i < blockCount; i++)
    {
        DrawBlock(&blocks[i]);
    }
}

