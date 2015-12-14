/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "PlatformFairy.hpp"

namespace chisa {
using namespace cinamo;

Handler<Quartet> PlatformFairy::quartet()
{
	if(!this->quartet_){
		this->quartet_ = this->createQuartet();
	}
	return this->quartet_;
}

Handler<JoystickManager> PlatformFairy::joystickManager()
{
	if(!this->joystickManager_){
		this->joystickManager_ = this->createJoystickManager();
	}
	return this->joystickManager_;
}

}
