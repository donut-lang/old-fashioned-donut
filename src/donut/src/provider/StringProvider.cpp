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

#include <sstream>
#include <tarte/String.h>
#include <donut/object/Heap.h>
#include <donut/object/StringObject.h>
#include <donut/provider/StringProvider.h>

namespace donut {
using namespace tarte;

static const std::string TAG("StringProvider");

StringProvider::StringProvider(Handler<Heap> const& heap )
:HeapProviderImpl<StringObject>(heap, "String")
{
	this->registerPureNativeClosure("toInteger", std::function<int(StringObject*)>([&](StringObject* self) {
		return parseAsInt<int>(self->toString(this->heap().lock()), 0);
	}));
	this->registerPureNativeClosure("toFloat", std::function<float(StringObject*)>([&](StringObject* self) {
		return parseAs<float>(self->toString(this->heap().lock()));
	}));
	this->registerPureNativeClosure("toBoolean", std::function<bool(StringObject*)>([&](StringObject* self) {
		return parseAs<bool>(self->toString(this->heap().lock()));
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
	this->registerPureNativeClosure("toString", std::function<StringObject*(StringObject*)>([&](StringObject* self) {
		return self;
	}));
}

}
