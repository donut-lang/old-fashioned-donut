/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cinamo/Handler.h>

namespace chisa {
using namespace cinamo;

class Joystick;
class JoystickManager : public HandlerBody<JoystickManager> {
private:
	std::vector<HandlerW<Joystick> > joysticks_;
public:
	JoystickManager();
	virtual ~JoystickManager() noexcept = default;
	inline bool onFree() const noexcept { return false; }
public:
	inline unsigned int numJoysticks() noexcept { return this->numJoysticksImpl(); }
	Handler<Joystick> joystick( unsigned int id );
protected:
	virtual unsigned int numJoysticksImpl() noexcept = 0;
	virtual Handler<Joystick> joystickImpl( unsigned int id ) = 0;
};

}
