/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <donut/object/Heap.h>
#include <donut/object/FloatObject.h>
#include <donut/provider/FloatProvider.h>

namespace donut {
using namespace cinamo;

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
		return ::cinamo::toString(self);
	});
}

}
