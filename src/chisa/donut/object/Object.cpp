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

#include "ObjectPool.h"
#include "Object.h"
#include "../Exception.h"

namespace chisa {
namespace donut {

const std::string TAG("Object");

Object::Object(ObjectPool* const pool)
:pool_(pool)
{

}

std::string Object::toString(ObjectPool* const pool) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toStringImpl();
	case Tag::Int:
		return pool->intProxy().toString(this);
	case Tag::Bool:
		return pool->boolProxy().toString(this);
	case Tag::Null:
		return pool->nullProxy().toString(this);
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

int Object::toInt(ObjectPool* const pool) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toIntImpl();
	case Tag::Int:
		return pool->intProxy().toInt(this);
	case Tag::Bool:
		return pool->boolProxy().toInt(this);
	case Tag::Null:
		return pool->nullProxy().toInt(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

float Object::toFloat(ObjectPool* const pool) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toFloatImpl();
	case Tag::Int:
		return pool->intProxy().toFloat(this);
	case Tag::Bool:
		return pool->boolProxy().toFloat(this);
	case Tag::Null:
		return pool->nullProxy().toFloat(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::toBool(ObjectPool* const pool) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toBoolImpl();
	case Tag::Int:
		return pool->intProxy().toBool(this);
	case Tag::Bool:
		return pool->boolProxy().toBool(this);
	case Tag::Null:
		return pool->nullProxy().toBool(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::have(ObjectPool* const pool, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->haveImpl(name);
	case Tag::Int:
		return pool->intProxy().have(this, name);
	case Tag::Bool:
		return pool->boolProxy().have(this, name);
	case Tag::Null:
		return pool->nullProxy().have(this, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::store(ObjectPool* const pool, const std::string& name, Handler<Object> obj)
{
	switch(this->tag()){
	case Tag::Obj:
		return obj;
	case Tag::Int:
		return pool->intProxy().store(this, name, obj);
	case Tag::Bool:
		return pool->boolProxy().store(this, name, obj);
	case Tag::Null:
		return pool->nullProxy().store(this, name, obj);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::load(ObjectPool* const pool, const std::string& name)
{
	switch(this->tag()){
	case Tag::Obj:
		return this->loadImpl(name);
	case Tag::Int:
		return pool->intProxy().load(this, name);
	case Tag::Bool:
		return pool->boolProxy().load(this, name);
	case Tag::Null:
		return pool->nullProxy().load(this, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

}}
