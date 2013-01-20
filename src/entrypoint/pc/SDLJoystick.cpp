/**
 * Chisa
 * Copyright (C) 2013 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SDLJoystick.h"

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
