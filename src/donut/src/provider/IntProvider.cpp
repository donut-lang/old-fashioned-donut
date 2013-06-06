/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/provider/IntProvider.h>

namespace donut {
using namespace cinamo;

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
	this->registerPureNativeClosure("opEq", [&](int self, Object* obj) {
		return obj->isInt() && self == obj->toInt(this->heap().lock());
	});
	this->registerPureNativeClosure("opNe", [&](int self, Object* obj) {
		return obj->isInt() && self != obj->toInt(this->heap().lock());
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
