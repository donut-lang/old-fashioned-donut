/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "JoystickManager.hpp"
#include "Joystick.hpp"

namespace chisa {

JoystickManager::JoystickManager()
{
}

Handler<Joystick> JoystickManager::joystick( unsigned int id )
{
	if(id < this->joysticks_.size()) {
		HandlerW<Joystick>& stick = this->joysticks_[id];
		if(!stick.expired()) {
			return stick.lock();
		}
	}
	Handler<Joystick> newOne = joystickImpl(id);
	this->joysticks_.resize(id+1);
	this->joysticks_[id] = newOne;
	return newOne;
}

}
