/**
 * Donut
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

#include <cinamo/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/object/Object.h>
#include <donut/provider/IntProvider.h>
#include <donut/provider/BoolProvider.h>
#include <donut/provider/NullProvider.h>
#include <donut/object/StringObject.h>
#include <donut/object/FloatObject.h>

namespace donut {

static const std::string TAG("Object");
static const std::string INDEX_STR[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50"};
static std::string toName( int const& id){
	return id >= 0 && static_cast<unsigned int>(id) < (sizeof(INDEX_STR)/sizeof(std::string)) ? INDEX_STR[id] : format("%d", id);
}

std::string Object::repr(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->reprImpl(heap);
	case Tag::Int:
		return heap->intProvider()->repr(this);
	case Tag::Bool:
		return heap->boolProvider()->repr(this);
	case Tag::Null:
		return heap->nullProvider()->repr(this);
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

std::string Object::print(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		return this->printImpl(heap);
	case Tag::Int:
		return heap->intProvider()->print(this);
	case Tag::Bool:
		return heap->boolProvider()->print(this);
	case Tag::Null:
		return heap->nullProvider()->print(this);
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

std::string const& Object::toString(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj: {
		Handler<const StringObject> val(this->toStringObjectImpl());
		if(val){
			return val->value();
		}else{
			DONUT_EXCEPTION(Exception, "Could not cast %s to string implicitly", this->reprImpl(heap).c_str());
		}
	}
	case Tag::Int:
		DONUT_EXCEPTION(Exception, "Could not cast %s to string implicitly", heap->intProvider()->repr(this).c_str());
	case Tag::Bool:
		DONUT_EXCEPTION(Exception, "Could not cast %s to string implicitly", heap->boolProvider()->repr(this).c_str());
	case Tag::Null:
		DONUT_EXCEPTION(Exception, "Could not cast %s to string implicitly", heap->nullProvider()->repr(this).c_str());
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

int Object::toInt(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		DONUT_EXCEPTION(Exception, "Could not cast %s to int implicitly", this->reprImpl(heap).c_str());
	case Tag::Int:
		return heap->intProvider()->toInt(this);
	case Tag::Bool:
		DONUT_EXCEPTION(Exception, "Could not cast %s to int implicitly", heap->boolProvider()->repr(this).c_str());
	case Tag::Null:
		DONUT_EXCEPTION(Exception, "Could not cast %s to int implicitly", heap->nullProvider()->repr(this).c_str());
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

float const& Object::toFloat(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj: {
		Handler<const FloatObject> val(this->toFloatObjectImpl());
		if(val){
			return val->value();
		}else{
			DONUT_EXCEPTION(Exception, "Could not cast %s to float implicitly", this->reprImpl(heap).c_str());
		}
	}
	case Tag::Int:
		DONUT_EXCEPTION(Exception, "Could not cast %s to float implicitly", heap->intProvider()->repr(this).c_str());
	case Tag::Bool:
		DONUT_EXCEPTION(Exception, "Could not cast %s to float implicitly", heap->boolProvider()->repr(this).c_str());
	case Tag::Null:
		DONUT_EXCEPTION(Exception, "Could not cast %s to float implicitly", heap->nullProvider()->repr(this).c_str());
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::toBool(Handler<Heap> const& heap) const
{
	switch(this->tag()){
	case Tag::Obj:
		DONUT_EXCEPTION(Exception, "Could not cast %s to bool implicitly", this->reprImpl(heap).c_str());
	case Tag::Int:
		DONUT_EXCEPTION(Exception, "Could not cast %s to bool implicitly", heap->intProvider()->repr(this).c_str());
	case Tag::Bool:
		return heap->boolProvider()->toBool(this);
	case Tag::Null:
		DONUT_EXCEPTION(Exception, "Could not cast %s to bool implicitly", heap->nullProvider()->repr(this).c_str());
	default:
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<NativeClosureObject> Object::tryCastToNativeClosureObject()
{
	return this->isObject() ? this->tryCastToNativeClosureObjectImpl() : Handler<NativeClosureObject>();
}

Handler<DonutClosureObject> Object::tryCastToDonutClosureObject()
{
	return this->isObject() ? this->tryCastToDonutClosureObjectImpl() : Handler<DonutClosureObject>();
}


bool Object::has(Handler<Heap> const& heap, std::string const& name) const
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
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

bool Object::hasOwn(Handler<Heap> const& heap, std::string const& name) const
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
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::set(Handler<Heap> const& heap, std::string const& name, Handler<Object> const& obj)
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
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

Handler<Object> Object::get(Handler<Heap> const& heap, std::string const& name) const
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
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

std::string Object::providerName(Handler<Heap> const& heap) const
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
		DONUT_EXCEPTION(Exception, "[BUG] Unknwon object tag: %d", this->tag());
	}
}

object_desc_t Object::toDescriptor() const noexcept
{
	if( this->isObject() ) {
		return this ? this->toDescriptorImpl() : 0;
	}else{
		return static_cast<object_desc_t>( reinterpret_cast<intptr_t>(this) );
	}
}

Handler<Object> Object::set(Handler<Heap> const& heap, int const& idx, Handler<Object> const& obj)
{
	return this->set(heap, toName(idx), obj);
}
Handler<Object> Object::get(Handler<Heap> const& heap, int const& idx) const
{
	return this->get(heap, toName(idx));
}

bool Object::has(Handler<Heap> const& heap, int const& idx) const
{
	return this->has(heap, toName(idx));
}

bool Object::hasOwn(Handler<Heap> const& heap, int const& idx) const
{
	return this->has(heap, toName(idx));
}

Handler<const StringObject> Object::toStringObjectImpl() const
{
	return Handler<const StringObject>();
}
Handler<const FloatObject> Object::toFloatObjectImpl() const
{
	return Handler<const FloatObject>();
}
Handler<NativeClosureObject> Object::tryCastToNativeClosureObjectImpl()
{
	return Handler<NativeClosureObject>();
}
Handler<DonutClosureObject> Object::tryCastToDonutClosureObjectImpl()
{
	return Handler<DonutClosureObject>();
}

bool Object::isString() const {
	return bool(this->toStringObjectImpl());
}
bool Object::isFloat() const {
	return bool(this->toFloatObjectImpl());
}

}
