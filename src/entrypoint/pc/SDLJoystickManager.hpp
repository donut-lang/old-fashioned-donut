/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../../chisa/input/JoystickManager.hpp"

namespace chisa {

class SDLJoystickManager: public JoystickManager {
public:
	SDLJoystickManager() = default;
	virtual ~SDLJoystickManager() noexcept = default;
private:
	virtual unsigned int numJoysticksImpl() noexcept;
	virtual Handler<Joystick> joystickImpl(unsigned int id);
};


}
