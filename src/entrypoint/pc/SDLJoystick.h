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

#pragma once
#include <SDL2/SDL.h>
#include "../../chisa/input/Joystick.h"

namespace chisa {

class SDLJoystick: public Joystick {
private:
	SDL_Joystick* joy_;
public:
	SDLJoystick(Handler<JoystickManager> manager, SDL_Joystick* joy)
			: Joystick(manager), joy_(joy)
	{

	}
	virtual ~SDLJoystick() noexcept
	{
		SDL_JoystickClose(joy_);
	}
protected:
	virtual unsigned int numAxesImpl() noexcept
	{
		return SDL_JoystickNumAxes(joy_);
	}
	virtual unsigned int numBallsImpl() noexcept
	{
		return SDL_JoystickNumBalls(joy_);
	}
	virtual unsigned int numButtonsImpl() noexcept
	{
		return SDL_JoystickNumButtons(joy_);
	}
protected:
	virtual int axisImpl(unsigned int const& idx) noexcept
	{
		return SDL_JoystickGetAxis(joy_, idx);
	}
	virtual void ballImpl(unsigned int const& idx, int& dx, int& dy)
	{
		SDL_JoystickGetBall(joy_, idx, &dx, &dy);
	}
	virtual bool buttonImpl(unsigned int const& idx) noexcept
	{
		return SDL_JoystickGetButton(joy_, idx) != 0;
	}
protected:
	virtual void updateImpl() noexcept
	{
		SDL_JoystickUpdate();
	}
};

}
