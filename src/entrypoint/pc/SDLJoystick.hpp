/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <SDL2/SDL.h>
#include "../../chisa/input/Joystick.hpp"

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
