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

#include "Provider.h"
#include "../../util/StringUtil.h"
#include "../object/Heap.h"
#include "../Exception.h"
#include <tinyxml2.h>
#include <sstream>

namespace chisa {
namespace donut {

static const std::string TAG("StringProvider");

StringProvider::StringProvider(const Handler<Heap>& heap )
:NativeObjectProvider(heap, "String")
{
	this->registerPureNativeClosure("toInteger", std::function<int(StringObject*)>([&](StringObject* self) {
		return util::parseInt(self->toString(this->heap().lock()), 0);
	}));
	this->registerPureNativeClosure("toFloat", std::function<float(StringObject*)>([&](StringObject* self) {
		return util::parseFloat(self->toString(this->heap().lock()));
	}));
	this->registerPureNativeClosure("toBoolean", std::function<bool(StringObject*)>([&](StringObject* self) {
		return util::parseBool(self->toString(this->heap().lock()));
	}));
	this->registerPureNativeClosure("opAdd", std::function<std::string(StringObject*,StringObject*)>([&](StringObject* self, StringObject* other) {
		std::string const str = self->toString(this->heap().lock());
		std::string const ostr = other->toString(this->heap().lock());
		return str + ostr;
	}));
	this->registerPureNativeClosure("opMul", std::function<std::string(StringObject*, int)>([&](StringObject* self, int times) {
		std::string const str = self->toString(this->heap().lock());
		std::stringstream ss;
		for(int i=0;i<times;++i){
			ss << str;
		}
		return ss.str();
	}));
}

util::XValue StringProvider::saveImpl(Handler<HeapObject> const& obj)
{
	if(Handler<Heap> heap = this->heap().lock()){
		return util::XValue( obj->toString(heap) );
	}else{
		throw DonutException(__FILE__, __LINE__, "[BUG] heap was already dead!");
	}
}
Handler<HeapObject> StringProvider::loadImpl(util::XValue const& data)
{
	if(Handler<Heap> heap = this->heap().lock()){
		return heap->createStringObject( data.as<util::XString>() );
	}else{
		throw DonutException(__FILE__, __LINE__, "[BUG] heap was already dead!");
	}
}

}}
