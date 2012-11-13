/**
 * Chisa
 * Copyright (C) 2012 psi
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

#include "World.h"
#include "Object.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"
#include <cstdlib>
#include <algorithm>
#include <cstring>

namespace chisa {
namespace donut {

const std::string TAG("FloatObject");

FloatObject::FloatObject(World* const world, const float val)
:BaseObject(world), val_(val)
{
}

FloatObject::FloatObject(World* const world)
:BaseObject(world), val_(NAN)
{
	this->store(world, "__proto__", world->objectPrototype());
}

std::string FloatObject::toStringImpl() const
{
	return util::format("%f", this->val_);
}

int FloatObject::toIntImpl() const
{
	return static_cast<int>(this->val_);
}

float FloatObject::toFloatImpl() const
{
	return this->val_;
}

}}
