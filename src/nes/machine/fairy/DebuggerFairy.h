/*
 * VideoFairy.h
 *
 *  Created on: 2011/08/23
 *      Author: psi
 */

#pragma once

#include <stdint.h>
#include <cstddef>
#include <tarte/XArchiver.h>
#include "../exception/EmulatorException.h"

namespace nes {

class DebuggerFairy
{
protected:
	DebuggerFairy() noexcept = default;
	virtual ~DebuggerFairy() noexcept = default;
public:
	virtual void onBreak() = 0;
	virtual void onStep() = 0;
};

}
