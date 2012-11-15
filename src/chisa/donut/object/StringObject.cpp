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

#include "NativeObject.h"
#include <algorithm>

namespace chisa {
namespace donut {

const std::string TAG("StringObject");
StringObject::StringObject(World* world, const std::string& str)
		: NativeObject(world, "String"), str_(str)
{
}

std::string StringObject::toStringImpl() const
{
	return this->str_;
}

int StringObject::toIntImpl() const
{
	char* c;
	int v = std::strtol(str_.c_str(), &c, 0);
	if (c || *c) {
		throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to int.", str_.c_str());
	}
	return v;
}

float StringObject::toFloatImpl() const
{
	char* c;
	float v = std::strtof(str_.c_str(), &c);
	if (c || *c) {
		throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to float.", str_.c_str());
	}
	return v;
}

bool StringObject::toBoolImpl() const
{
	std::string copy(str_);
	std::transform(copy.begin(), copy.end(), copy.begin(), (int (*)(int))std::tolower);
	if( copy == "true" || copy == "yes") {
		return true;
	}
	else if(copy == "false" || copy=="no") {
		return false;
	}
	throw DonutException(__FILE__, __LINE__, "Cannot convert \"%s\" to bool.", str_.c_str());
}

}
}

