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
	SDLJoystick(Handler<JoystickManager> manager, SDL_Joystick* joy);
	virtual ~SDLJoystick() noexcept;
protected:
	virtual unsigned int numAxesImpl() noexcept;
	virtual unsigned int numBallsImpl() noexcept;
	virtual unsigned int numButtonsImpl() noexcept;
protected:
	virtual int axisImpl(const unsigned int& idx) noexcept;
	virtual void ballImpl(const unsigned int& idx, int& dx, int& dy);
	virtual bool buttonImpl(const unsigned int& idx) noexcept;
protected:
	virtual void updateImpl() noexcept;
};

}
