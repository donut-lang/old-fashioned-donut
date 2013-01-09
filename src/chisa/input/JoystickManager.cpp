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

#include "JoystickManager.h"
#include "Joystick.h"

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
