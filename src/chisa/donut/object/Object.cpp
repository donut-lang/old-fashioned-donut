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
#include "ObjectBase.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("Object");
static const std::string INDEX_STR[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50"};
static std::string toName( const int& id){
	return id >= 0 && static_cast<unsigned int>(id) < (sizeof(INDEX_STR)/sizeof(std::string)) ? INDEX_STR[id] : util::format("%d", id);
}


std::string Object::toString(World* const world) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toStringImpl();
	case Tag::Int:
		return world->intProvider().toString(world, this);
	case Tag::Bool:
		return world->boolProvider().toString(world, this);
	case Tag::Null:
		return world->nullProvider().toString(world, this);
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

int Object::toInt(World* const world) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toIntImpl();
	case Tag::Int:
		return world->intProvider().toInt(world, this);
	case Tag::Bool:
		return world->boolProvider().toInt(world, this);
	case Tag::Null:
		return world->nullProvider().toInt(world, this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

float Object::toFloat(World* const world) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toFloatImpl();
	case Tag::Int:
		return world->intProvider().toFloat(world, this);
	case Tag::Bool:
		return world->boolProvider().toFloat(world, this);
	case Tag::Null:
		return world->nullProvider().toFloat(world, this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::toBool(World* const world) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toBoolImpl();
	case Tag::Int:
		return world->intProvider().toBool(world, this);
	case Tag::Bool:
		return world->boolProvider().toBool(world, this);
	case Tag::Null:
		return world->nullProvider().toBool(world, this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::have(World* const world, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->haveImpl(name);
	case Tag::Int:
		return world->intProvider().have(world, this, name);
	case Tag::Bool:
		return world->boolProvider().have(world, this, name);
	case Tag::Null:
		return world->nullProvider().have(world, this, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::haveOwn(World* const world, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->haveOwnImpl(name);
	case Tag::Int:
		return world->intProvider().haveOwn(world, this, name);
	case Tag::Bool:
		return world->boolProvider().haveOwn(world, this, name);
	case Tag::Null:
		return world->nullProvider().haveOwn(world, this, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::store(World* const world, const std::string& name, Handler<Object> obj)
{
	switch(this->tag()){
	case Tag::Obj:
		return this->storeImpl(name, obj);
	case Tag::Int:
		return world->intProvider().store(world, this, name, obj);
	case Tag::Bool:
		return world->boolProvider().store(world, this, name, obj);
	case Tag::Null:
		return world->nullProvider().store(world, this, name, obj);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::load(World* const world, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->loadImpl(name);
	case Tag::Int:
		return world->intProvider().load(world, this, name);
	case Tag::Bool:
		return world->boolProvider().load(world, this, name);
	case Tag::Null:
		return world->nullProvider().load(world, this, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

std::string Object::providerName(World* const world) const
{
	switch(this->tag()){
	case Tag::Obj:
		return "OBJECT"; //XXX
	case Tag::Int:
		return world->intProvider().name();
	case Tag::Bool:
		return world->boolProvider().name();
	case Tag::Null:
		return world->nullProvider().name();
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}


Handler<Object> Object::store(World* const pool, const int& idx, Handler<Object> obj)
{
	return this->store(pool, toName(idx), obj);
}
Handler<Object> Object::load(World* const pool, const int& idx) const
{
	return this->load(pool, toName(idx));
}

bool Object::have(World* const pool, const int& idx) const
{
	return this->have(pool, toName(idx));
}

}}
