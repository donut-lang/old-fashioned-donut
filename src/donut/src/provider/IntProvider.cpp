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

#include <tarte/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/provider/IntProvider.h>

namespace donut {
using namespace tarte;

static const std::string TAG("IntProvider");

IntProvider::IntProvider(Handler<Heap> const& heap)
:Provider(heap, "Integer")
{
	this->registerPureNativeClosure("opAdd", [&](int self, int v) {
		return self + v;
	});
	this->registerPureNativeClosure("opBitAnd", [&](int self, int v) {
		return self & v;
	});
	this->registerPureNativeClosure("opBitOr", [&](int self, int v) {
		return self | v;
	});
	this->registerPureNativeClosure("opSub", [&](int self, int v) {
		return self-v;
	});
	this->registerPureNativeClosure("opMul", [&](int self, int v) {
		return self*v;
	});
	this->registerPureNativeClosure("opDiv", [&](int self, int v) {
		return self/v;
	});
	this->registerPureNativeClosure("opPlus", [&](int self) {
		return +self;
	});
	this->registerPureNativeClosure("opMinus", [&](int self) {
		return -self;
	});
	this->registerPureNativeClosure("toFloat", [&](int self) {
		return static_cast<float>(self);
	});
	this->registerPureNativeClosure("opMod", [&](int self, int v) {
		return self % v;
	});
	this->registerPureNativeClosure("opLt", [&](int self, int v) {
		return self < v;
	});
	this->registerPureNativeClosure("opLe", [&](int self, int v) {
		return self <= v;
	});
	this->registerPureNativeClosure("opGt", [&](int self, int v) {
		return self > v;
	});
	this->registerPureNativeClosure("opGe", [&](int self, int v) {
		return self >= v;
	});
	this->registerPureNativeClosure("opEq", [&](int self, int v) {
		return self == v;
	});
	this->registerPureNativeClosure("opNe", [&](int self, int v) {
		return self != v;
	});
	this->registerPureNativeClosure("toString", [&](int self) {
		return toString(self);
	});
}

std::string IntProvider::repr(const Object* ptr) const
{
	return std::string("int:")+toString(IntProvider::fromPointer(ptr));
}

std::string IntProvider::print(const Object* ptr) const
{
	return toString(IntProvider::fromPointer(ptr));
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
