#ifndef EVENTS_H
#define EVENTS_H

#include <stdbool.h>
#include "player.h"
#include <SDL2/SDL_types.h>
#include "debug.h"

#define CAMERA_MOVE_LEFT        (0b00000000001)
#define CAMERA_MOVE_RIGHT       (0b00000000010)
#define CAMERA_MOVE_FORWARD     (0b00000000100)
#define CAMERA_MOVE_BACKWARD    (0b00000001000)
#define CAMERA_MOVE_UP          (0b00000010000)
#define CAMERA_MOVE_DOWN        (0b00000100000)
#define CAMERA_YAW_LEFT         (0b00001000000)
#define CAMERA_YAW_RIGHT        (0b00010000000)
#define CAMERA_PITCH_UP         (0b00100000000)
#define CAMERA_PITCH_DOWN       (0b01000000000)
#define SHADER_RELOAD_REQUESTED (0b10000000000)

NODISCARD
/**
 * Polls all incoming events through SDL
 * 
 * @param shouldClose Address to a bool which is set if SDL_QUIT (etc) is received
 * @param player the player whose camera's aspect ratio may be updated
 * @param buttonsHeld Uint32 bitfield of any pertinent buttons held (to be sent to moveCamera)
 * @param mouseState Uint32 bitfield of any pertinent mouse information (retrieved from SDL_GetMouseState)
 * 
 * @returns The number of events polled, or (-1) if error
 */
int handleEvents(bool *shouldClose, player_t player, Uint32 * buttonsHeld, Uint32 * mouseState);

#endif