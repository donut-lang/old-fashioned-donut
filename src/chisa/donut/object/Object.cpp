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

#include "Heap.h"
#include "Object.h"
#include "../Exception.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace donut {

static const std::string TAG("Object");
static const std::string INDEX_STR[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50"};
static std::string toName( const int& id){
	return id >= 0 && static_cast<unsigned int>(id) < (sizeof(INDEX_STR)/sizeof(std::string)) ? INDEX_STR[id] : util::format("%d", id);
}


std::string Object::toString(const Handler<Heap>& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toStringImpl(heap);
	case Tag::Int:
		return heap->intProvider()->toString(this);
	case Tag::Bool:
		return heap->boolProvider()->toString(this);
	case Tag::Null:
		return heap->nullProvider()->toString(this);
	default:
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

int Object::toInt(const Handler<Heap>& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toIntImpl(heap);
	case Tag::Int:
		return heap->intProvider()->toInt(this);
	case Tag::Bool:
		return heap->boolProvider()->toInt(this);
	case Tag::Null:
		return heap->nullProvider()->toInt(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

float Object::toFloat(const Handler<Heap>& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toFloatImpl(heap);
	case Tag::Int:
		return heap->intProvider()->toFloat(this);
	case Tag::Bool:
		return heap->boolProvider()->toFloat(this);
	case Tag::Null:
		return heap->nullProvider()->toFloat(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::toBool(const Handler<Heap>& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->toBoolImpl(heap);
	case Tag::Int:
		return heap->intProvider()->toBool(this);
	case Tag::Bool:
		return heap->boolProvider()->toBool(this);
	case Tag::Null:
		return heap->nullProvider()->toBool(this);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::has(const Handler<Heap>& heap, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->hasImpl(heap, name);
	case Tag::Int:
		return heap->intProto()->hasImpl(heap, name);
	case Tag::Bool:
		return heap->boolProto()->hasImpl(heap, name);
	case Tag::Null:
		return heap->nullProto()->hasImpl(heap, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::hasOwn(const Handler<Heap>& heap, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->hasOwnImpl(heap, name);
	case Tag::Int:
		return heap->intProto()->hasOwnImpl(heap, name);
	case Tag::Bool:
		return heap->boolProto()->hasOwnImpl(heap, name);
	case Tag::Null:
		return heap->nullProto()->hasOwnImpl(heap, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::set(const Handler<Heap>& heap, const std::string& name, Handler<Object> obj)
{
	switch(this->tag()){
	case Tag::Obj:
		return this->setImpl(heap, name, obj);
	case Tag::Int:
		heap->log().w(TAG, "Failed to store value to int object.");
		return obj;
	case Tag::Bool:
		heap->log().w(TAG, "Failed to store value to bool object.");
		return obj;
	case Tag::Null:
		heap->log().w(TAG, "Failed to store value to null object.");
		return obj;
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::get(const Handler<Heap>& heap, const std::string& name) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->getImpl(heap, name);
	case Tag::Int:
		return heap->intProto()->getImpl(heap, name);
	case Tag::Bool:
		return heap->boolProto()->getImpl(heap, name);
	case Tag::Null:
		return heap->nullProto()->getImpl(heap, name);
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

std::string Object::providerName(const Handler<Heap>& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->providerNameImpl(heap);
	case Tag::Int:
		return heap->intProvider()->name();
	case Tag::Bool:
		return heap->boolProvider()->name();
	case Tag::Null:
		return heap->nullProvider()->name();
	default:
		throw DonutException(__FILE__, __LINE__, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

object_desc_t Object::toDescriptor() const noexcept
{
	if( this->isObject() ) {
		return this->toDescriptorImpl();
	}else{
		return static_cast<object_desc_t>( reinterpret_cast<intptr_t>(this) );
	}
}

Handler<Object> Object::set(const Handler<Heap>& heap, const int& idx, Handler<Object> obj)
{
	return this->set(heap, toName(idx), obj);
}
Handler<Object> Object::get(const Handler<Heap>& heap, const int& idx) const
{
	return this->get(heap, toName(idx));
}

bool Object::has(const Handler<Heap>& heap, const int& idx) const
{
	return this->has(heap, toName(idx));
}

bool Object::hasOwn(const Handler<Heap>& heap, const int& idx) const
{
	return this->has(heap, toName(idx));
}

}}
