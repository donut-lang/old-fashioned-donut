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

#include "Object.h"

namespace chisa {
namespace donut {

const std::string TAG("Object");

Object::Object(ObjectPool* const pool)
:pool_(pool)
{
	// TODO Auto-generated constructor stub

}

std::string Object::toString() const
{
	switch(this->tag()){
	case Tag::Obj:
		break;
	case Tag::Int:
		break;
	case Tag::Bool:
		break;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

int Object::toInt() const
{
	switch(this->tag()){
	case Tag::Obj:
		break;
	case Tag::Int:
		break;
	case Tag::Bool:
		break;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

float Object::toFloat() const
{
	switch(this->tag()){
	case Tag::Obj:
		break;
	case Tag::Int:
		break;
	case Tag::Bool:
		break;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::toBool() const
{
	switch(this->tag()){
	case Tag::Obj:
		break;
	case Tag::Int:
		break;
	case Tag::Bool:
		break;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::store(const std::string& name, Handler<Object> obj)
{
	switch(this->tag()){
	case Tag::Obj:
		break;
	case Tag::Int:
		this->pool_->log().w(TAG, "Failed to store value to int object.");
		return obj;
	case Tag::Bool:
		this->pool_->log().w(TAG, "Failed to store value to boolean object.");
		return obj;
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::load(const std::string& name)
{
}

}}
