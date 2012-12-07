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

namespace chisa {
namespace donut {

static const std::string TAG("IntProvider");

IntProvider::IntProvider(const Handler<Heap>& heap)
:NativeObjectProvider(heap, "Integer")
{
	this->registerPureNativeClosure("opAdd", std::function<int(int, int)>([&](int self, int v) {
		return self + v;
	}));
	this->registerPureNativeClosure("opBitAnd", std::function<int(int, int)>([&](int self, int v) {
		return self & v;
	}));
	this->registerPureNativeClosure("opBitOr", std::function<int(int, int)>([&](int self, int v) {
		return self | v;
	}));
	this->registerPureNativeClosure("opSub", std::function<int(int, int)>([&](int self, int v) {
		return self-v;
	}));
	this->registerPureNativeClosure("opMul", std::function<int(int, int)>([&](int self, int v) {
		return self*v;
	}));
	this->registerPureNativeClosure("opDiv", std::function<int(int, int)>([&](int self, int v) {
		return self/v;
	}));
	this->registerPureNativeClosure("opPlus", std::function<int(int)>([&](int self) {
		return +self;
	}));
	this->registerPureNativeClosure("opMinus", std::function<int(int)>([&](int self) {
		return -self;
	}));
	this->registerPureNativeClosure("toFloat", std::function<float(int)>([&](int self) {
		return static_cast<float>(self);
	}));
	this->registerPureNativeClosure("opMod", std::function<int(int, int)>([&](int self, int v) {
		return self % v;
	}));
	this->registerPureNativeClosure("opLt", std::function<bool(int, int)>([&](int self, int v) {
		return self < v;
	}));
	this->registerPureNativeClosure("opLe", std::function<bool(int, int)>([&](int self, int v) {
		return self <= v;
	}));
	this->registerPureNativeClosure("opGt", std::function<bool(int, int)>([&](int self, int v) {
		return self > v;
	}));
	this->registerPureNativeClosure("opGe", std::function<bool(int, int)>([&](int self, int v) {
		return self >= v;
	}));
	this->registerPureNativeClosure("opEq", std::function<bool(int, int)>([&](int self, int v) {
		return self == v;
	}));
	this->registerPureNativeClosure("opNe", std::function<bool(int, int)>([&](int self, int v) {
		return self != v;
	}));
	this->registerPureNativeClosure("toString", std::function<std::string(int)>([&](int self) {
		return util::toString(self);
	}));
}

std::string IntProvider::toString(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Integer to String implicitly. Use toString()");
}

int IntProvider::toInt(const Object* ptr) const
{
	return fromPointer(ptr);
}

float IntProvider::toFloat(const Object* ptr) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Integer to Float implicitly. Use toFloat()");
}

bool IntProvider::toBool(const Object* ptr) const
{

	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Integer to Boolean.");
}

Handler<Object> IntProvider::create( const int& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( IntProvider::toPointer(val) );
}

util::XValue IntProvider::saveImpl(Handler<Object> const& obj)
{
	return util::XValue( IntProvider::fromPointer(obj.get()) );
}
Handler<Object> IntProvider::loadImpl(util::XValue const& data)
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( IntProvider::toPointer(data.as<int>()) );
}

}}
