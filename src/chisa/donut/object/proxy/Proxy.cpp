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

#include "Proxy.h"
#include "../World.h"
#include "../../Exception.h"

namespace chisa {
namespace donut {
static const std::string TAG("Proxy");

bool Proxy::have(const Object* ptr, const std::string& name) const
{
	return haveOwn(ptr, name) || this->world()->objectPrototype()->have(world(), name);
}

bool Proxy::haveOwn(const Object* ptr, const std::string& name) const
{
	return (this->closureMap_.find(name) != this->closureMap_.end());
}

Handler<Object> Proxy::load(const Object* ptr, const std::string& name)
{
	auto it = this->closureMap_.find(name);
	if(it != this->closureMap_.end()){
		return Handler<Object>::__internal__fromRawPointerWithoutCheck(it->second.get());
	}
	return world()->objectPrototype()->load(world(), name);
}

}}

