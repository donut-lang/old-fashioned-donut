/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Joystick.hpp"
#include "JoystickManager.hpp"

namespace chisa {

Joystick::Joystick(Handler<JoystickManager> const& manager)
:manager_(manager)
{
}

Handler<JoystickManager> Joystick::manager() const
{
	return this->manager_.lock();
}

}
