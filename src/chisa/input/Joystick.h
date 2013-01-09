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
#include <tarte/Handler.h>

namespace chisa {
using namespace tarte;

class JoystickManager;
class Joystick : public HandlerBody<Joystick>{
private:
	HandlerW<JoystickManager> manager_;
public:
	Joystick(Handler<JoystickManager> const& manager);
	virtual ~Joystick() noexcept = default;
	inline bool onFree() const noexcept { return false; }
public:
	Handler<JoystickManager> manager() const;
public:
	unsigned int numAxes() noexcept { return this->numAxesImpl(); };
	unsigned int numBalls() noexcept { return this->numBallsImpl(); };
	unsigned int numButtons() noexcept  { return this->numButtonsImpl(); };
public:
	int axis(unsigned int const& idx) noexcept { return this->axisImpl(idx); };
	void ball(unsigned int const& idx, int& dx, int& dy) noexcept { return this->ballImpl(idx,dx,dy); };
	bool button(unsigned int const& idx) noexcept  { return this->buttonImpl(idx); };
public:
	inline void update() noexcept { this->updateImpl(); }
protected:
	virtual unsigned int numAxesImpl() noexcept = 0;
	virtual unsigned int numBallsImpl() noexcept = 0;
	virtual unsigned int numButtonsImpl() noexcept = 0;
protected:
	virtual int axisImpl(unsigned int const& idx) noexcept = 0;
	virtual void ballImpl(unsigned int const& idx, int& dx, int& dy) = 0;
	virtual bool buttonImpl(unsigned int const& idx) noexcept = 0;
protected:
	virtual void updateImpl() noexcept = 0;
};

}
