#include "events.h"
#include "camera.h"
#include "game.h"
#include "render.h"
#include "main.h"

int handleEvents(bool *shouldClose, player_t player, Uint32 * buttonsHeld, Uint32 * mouseState)
{
	*mouseState = SDL_GetRelativeMouseState(&mx, &my);

	SDL_Event event;
	Uint32 eventCount = 0;
	while(SDL_PollEvent(&event))
	{
		eventCount++;
		switch(event.type)
		{
			case SDL_QUIT:
				(*shouldClose) = true;
				return eventCount;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					ww = event.window.data1;
					wh = event.window.data2;
					glViewport(0,0,ww,wh);
					if(!playerUpdateCameraAspectRatio(player)) return -1;
					break;
				}
			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_RETURN:
						toggleWireframe();
						break;
					case SDL_SCANCODE_ESCAPE:
						(*shouldClose) = true;
						return eventCount;
					case SDL_SCANCODE_W:
						(*buttonsHeld) |= CAMERA_MOVE_FORWARD;
						break;
					case SDL_SCANCODE_A:
						(*buttonsHeld) |= CAMERA_MOVE_LEFT;
						break;
					case SDL_SCANCODE_S:
						(*buttonsHeld) |= CAMERA_MOVE_BACKWARD;
						break;
					case SDL_SCANCODE_D:
						(*buttonsHeld) |= CAMERA_MOVE_RIGHT;
						break;
					case SDL_SCANCODE_LSHIFT:
						(*buttonsHeld) |= CAMERA_MOVE_DOWN;
						break;
					case SDL_SCANCODE_SPACE:
						(*buttonsHeld) |= CAMERA_MOVE_UP;
						break;
					case SDL_SCANCODE_LEFT:
						(*buttonsHeld) |= CAMERA_YAW_LEFT;
						break;
					case SDL_SCANCODE_RIGHT:
						(*buttonsHeld) |= CAMERA_YAW_RIGHT;
						break;
					case SDL_SCANCODE_UP:
						(*buttonsHeld) |= CAMERA_PITCH_UP;
						break;
					case SDL_SCANCODE_DOWN:
						(*buttonsHeld) |= CAMERA_PITCH_DOWN;
						break;
					case SDL_SCANCODE_H:
						(*buttonsHeld) |= SHADER_RELOAD_REQUESTED;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_W:
						(*buttonsHeld) &= ~CAMERA_MOVE_FORWARD;
						break;
					case SDL_SCANCODE_A:
						(*buttonsHeld) &= ~CAMERA_MOVE_LEFT;
						break;
					case SDL_SCANCODE_S:
						(*buttonsHeld) &= ~CAMERA_MOVE_BACKWARD;
						break;
					case SDL_SCANCODE_D:
						(*buttonsHeld) &= ~CAMERA_MOVE_RIGHT;
						break;
					case SDL_SCANCODE_LSHIFT:
						(*buttonsHeld) &= ~CAMERA_MOVE_DOWN;
						break;
					case SDL_SCANCODE_SPACE:
						(*buttonsHeld) &= ~CAMERA_MOVE_UP;
						break;
					case SDL_SCANCODE_LEFT:
						(*buttonsHeld) &= ~CAMERA_YAW_LEFT;
						break;
					case SDL_SCANCODE_RIGHT:
						(*buttonsHeld) &= ~CAMERA_YAW_RIGHT;
						break;
					case SDL_SCANCODE_UP:
						(*buttonsHeld) &= ~CAMERA_PITCH_UP;
						break;
					case SDL_SCANCODE_DOWN:
						(*buttonsHeld) &= ~CAMERA_PITCH_DOWN;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}

	return eventCount;
}