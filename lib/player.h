#include <cglm/types.h>
#include <SDL2/SDL_types.h>

#define PLAYER_HEIGHT (1.9)
#define PLAYER_EYE_HEIGHT (1.7)
#define PLAYER_WIDTH (0.75)

#define PLAYER_DEFAULT_SPEED (4.5)
#define PLAYER_SPRINT_SPEED (6.0)

typedef struct player *player_t;

/**
 * Initializes a player object with the given foot position. 
 * 
 * @param footPos the player's initial foot position
 * @return player_t 
 */
player_t initPlayer(vec3 footPos);

/**
 * Updates the player's eye camera aspect ratio using the global window size 
 * 
 * @param player the player to update
 * @return float 
 */
float playerUpdateCameraAspectRatio(player_t player);

/**
 * Renders the world, as seen by the given player 
 * 
 * @param player which player's view to render
 */
void playerRenderWorld(player_t player);

/**
 * Moves the player based on inputs in `buttonsHeld`
 * 
 * @param player the player to move
 * @param buttonsHeld the current inputs
 * @return 0 on success, non-zero on fail (getError)
 */
int movePlayer(player_t player, Uint32 buttonsHeld);