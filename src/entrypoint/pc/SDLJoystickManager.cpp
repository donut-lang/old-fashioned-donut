/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <SDL2/SDL.h>
#include "SDLJoystick.hpp"
#include "SDLJoystickManager.hpp"

namespace chisa {

unsigned int SDLJoystickManager::numJoysticksImpl() noexcept
{
	return SDL_NumJoysticks();
}

Handler<Joystick> SDLJoystickManager::joystickImpl(unsigned int id)
{
	Handler<Joystick> joy(new SDLJoystick(self(), SDL_JoystickOpen(id)));
	return joy;
}

}
