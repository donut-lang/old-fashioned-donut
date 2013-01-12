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
:Super(heap, "String")
{
	this->registerPureNativeClosure("toInteger", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<int>(self->toString(heap), 0);
	});
	this->registerPureNativeClosure("toFloat", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<float>(self->toString(heap));
	});
	this->registerPureNativeClosure("toBoolean", [this](StringObject* self) {
		Handler<Heap> heap(this->heap().lock());
		return parseAs<bool>(self->toString(heap));
	});
	this->registerPureNativeClosure("opAdd", [this](StringObject* self, Object* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->print(heap);
		return str + ostr;
	});
	this->registerPureNativeClosure("opEq", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str == ostr;
	});
	this->registerPureNativeClosure("opNe", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str != ostr;
	});
	this->registerPureNativeClosure("opLt", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str < ostr;
	});
	this->registerPureNativeClosure("opLe", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str <= ostr;
	});
	this->registerPureNativeClosure("opGt", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str > ostr;
	});
	this->registerPureNativeClosure("opGe", [this](StringObject* self, StringObject* other) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::string const ostr = other->toString(heap);
		return str >= ostr;
	});
	this->registerPureNativeClosure("opMul", [this](StringObject* self, int times) {
		Handler<Heap> heap(this->heap().lock());
		std::string const str = self->toString(heap);
		std::stringstream ss;
		for(int i=0;i<times;++i){
			ss << str;
		}
		return ss.str();
	});
	this->registerPureNativeClosure("toString", [this](StringObject* self) {
		return self;
	});
}

}
