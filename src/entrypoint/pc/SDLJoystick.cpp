/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "SDLJoystick.hpp"

namespace chisa {


SDLJoystick::SDLJoystick(Handler<JoystickManager> manager, SDL_Joystick* joy)
:Joystick(manager)
,joy_(joy)
{
}

SDLJoystick::~SDLJoystick() noexcept
{
	SDL_JoystickClose(joy_);
}

unsigned int SDLJoystick::numAxesImpl() noexcept
{
	return SDL_JoystickNumAxes(joy_);
}

unsigned int SDLJoystick::numBallsImpl() noexcept
{
	return SDL_JoystickNumBalls(joy_);
}

unsigned int SDLJoystick::numButtonsImpl() noexcept
{
	return SDL_JoystickNumButtons(joy_);
}

int SDLJoystick::axisImpl(const unsigned int& idx) noexcept
{
	return SDL_JoystickGetAxis(joy_, idx);
}

void SDLJoystick::ballImpl(const unsigned int& idx, int& dx, int& dy)
{
	SDL_JoystickGetBall(joy_, idx, &dx, &dy);
}

bool SDLJoystick::buttonImpl(const unsigned int& idx) noexcept
{
	return SDL_JoystickGetButton(joy_, idx) != 0;
}

void SDLJoystick::updateImpl() noexcept
{
	SDL_JoystickUpdate();
}

}
