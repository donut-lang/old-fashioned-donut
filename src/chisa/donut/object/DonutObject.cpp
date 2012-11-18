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

#include <sstream>
#include "World.h"
#include "Object.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("DonutObject");

DonutObject::DonutObject(World* world, const std::string& providerName)
:HeapObject(world, providerName)
{

}

DonutObject::DonutObject(World* world)
:HeapObject(world, "chisa::donut::DonutObject")
{

}

std::string DonutObject::toStringImpl() const
{
	return util::format("(DonutObject %p)", this);
}

std::string DonutObject::providerNameImpl() const
{
	return this->providerName();
}

int DonutObject::toIntImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert object to int.");
}

float DonutObject::toFloatImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to float.");
}

bool DonutObject::toBoolImpl() const
{
	throw DonutException(__FILE__, __LINE__, "Failed to convert to bool.");
}

bool DonutObject::haveImpl(const std::string& name) const
{
	return haveOwnImpl(name) || (haveOwnImpl("__proto__") && loadImpl("__proto__")->have(world(), name));
}

bool DonutObject::haveOwnImpl(const std::string& name) const
{
	auto it = this->slots_.find(name);
	return it != this->slots_.end() && it->second.have();
}

Handler<Object> DonutObject::storeImpl(const std::string& name, Handler<Object> obj)
{
	auto it = this->slots_.find(name);
	if(it == this->slots_.end()){
		this->slots_.insert( std::pair<std::string, Slot>(name, Slot(this->world(), obj.get())) );
	}else{
		it->second.store(obj.get());
	}
	return obj;
}

Handler<Object> DonutObject::loadImpl(const std::string& name) const
{
	auto it = this->slots_.find(name);
	if(it != this->slots_.end()){
		return Handler<Object>::__internal__fromRawPointerWithoutCheck(it->second.load());
	}
	if(this->haveOwnImpl("__proto__")){
		return loadImpl("__proto__")->load(world(), name);
	}
	{
		std::stringstream ss;
		ss << "\"" << name << "\" not found." << std::endl;
		ss << "available: " << std::endl;
		for(auto i : this->slots_){
			ss << "  -> " << i.first << std::endl;
		}
		throw DonutException(__FILE__, __LINE__, ss.str());
	}
}

void DonutObject::seekImpl(timestamp_t time)
{
	for(std::pair<const std::string, Slot>& it : this->slots_){
		it.second.seek(time);
	}
}

}}
