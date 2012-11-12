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

namespace chisa {
namespace donut {

static const std::string TAG("BaseObject");

BaseObject::BaseObject(World* world)
:world_(world)
{

}

std::string BaseObject::toStringImpl() const
{
	return util::format("(BaseObject %p)", this);
}

int BaseObject::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert object to int.");
}

float BaseObject::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to float.");
}

bool BaseObject::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to bool.");
}

bool BaseObject::haveImpl(const std::string& name) const
{
	return haveOwnImpl(name) || (haveOwnImpl("__proto__") && loadImpl("__proto__")->have(world(), name));
}

bool BaseObject::haveOwnImpl(const std::string& name) const
{
	auto it = this->slots_.find(name);
	return it != this->slots_.end() && it->second.have();
}

Handler<Object> BaseObject::storeImpl(const std::string& name, Handler<Object> obj)
{
	auto it = this->slots_.find(name);
	if(it == this->slots_.end()){
		this->slots_.insert( std::pair<std::string, Slot>(name, Slot(this->world(), obj.get())) );
	}else{
		it->second.store(obj.get());
	}
	return obj;
}

Handler<Object> BaseObject::loadImpl(const std::string& name) const
{
	auto it = this->slots_.find(name);
	if(it != this->slots_.end()){
		return Handler<Object>::__internal__fromRawPointerWithoutCheck(it->second.load());
	}
	if(this->haveOwnImpl("__proto__")){
		return loadImpl("__proto__")->load(world(), name);
	}
	throw DonutException(__FILE__, __LINE__, "%s not found.", name.c_str());
}

}}
