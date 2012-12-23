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

#include "IntProvider.h"
#include "../object/Heap.h"
#include "../Exception.h"
#include <tarte/String.h>

namespace donut {
using namespace tarte;

static const std::string TAG("IntProvider");

IntProvider::IntProvider(Handler<Heap> const& heap)
:Provider(heap, "Integer")
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
		return toString(self);
	}));
}

std::string IntProvider::repr(const Object* ptr) const
{
	return std::string("int:")+toString(IntProvider::fromPointer(ptr));
}

int IntProvider::toInt(const Object* ptr) const
{
	return fromPointer(ptr);
}

Handler<Object> IntProvider::create( int const& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( IntProvider::toPointer(val) );
}

}
