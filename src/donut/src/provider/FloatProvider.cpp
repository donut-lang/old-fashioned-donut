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
#include <donut/object/Heap.h>
#include <donut/object/FloatObject.h>
#include <donut/provider/FloatProvider.h>

namespace donut {
using namespace tarte;

static const std::string TAG("FloatProvider");

FloatProvider::FloatProvider(Handler<Heap> const& heap)
:Super(heap, "Float")
{
	this->registerPureNativeClosure("opAdd", [&](float self, float v) {
		return self+v;
	});
	this->registerPureNativeClosure("opSub", [&](float self, float v) {
		return self-v;
	});
	this->registerPureNativeClosure("opMul", [&](float self, float v) {
		return self*v;
	});
	this->registerPureNativeClosure("opDiv", [&](float self, float v) {
		return self/v;
	});
	this->registerPureNativeClosure("opPlus", [&](float self) {
		return self;
	});
	this->registerPureNativeClosure("opMinus", [&](float self) {
		return -self;
	});
	this->registerPureNativeClosure("opLt", [&](float self, float v) {
		return self < v;
	});
	this->registerPureNativeClosure("opLe", [&](float self, float v) {
		return self <= v;
	});
	this->registerPureNativeClosure("opGt", [&](float self, float v) {
		return self > v;
	});
	this->registerPureNativeClosure("opGe", [&](float self, float v) {
		return self >= v;
	});
	this->registerPureNativeClosure("opEq", [this](float self, Object* obj) {
		return (obj->isFloat()) && self == obj->toFloat(this->heap().lock());
	});
	this->registerPureNativeClosure("opNe", [this](float self, Object* obj) {
		return (!obj->isFloat()) || self != obj->toFloat(this->heap().lock());
	});
	this->registerPureNativeClosure("toInteger", [&](float self) {
		return static_cast<int>(self);
	});
	this->registerPureNativeClosure("toString", [&](float self) {
		return ::tarte::toString(self);
	});
}

}
