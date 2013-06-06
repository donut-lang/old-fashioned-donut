/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/String.h>
#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/provider/BoolProvider.h>

namespace donut {
using namespace cinamo;

static const std::string TAG("BoolProvider");

BoolProvider::BoolProvider(Handler<Heap> const& heap)
:Provider(heap, "Boolean")
{
	this->registerPureNativeClosure("opAnd", [&](bool self, bool v){
		return self && v;
	});
	this->registerPureNativeClosure("opOr", [&](bool self, bool v){
		return self || v;
	});
	this->registerPureNativeClosure("opNot", [&](bool self){
		return !self;
	});
	this->registerPureNativeClosure("opEq", [&](bool self, Object* obj){
		return obj->isBool() && obj->toBool(this->heap().lock()) == self;
	});
	this->registerPureNativeClosure("opNe", [&](bool self, Object* obj){
		return obj->isBool() && obj->toBool(this->heap().lock()) != self;
	});
	this->registerPureNativeClosure("toString", [&](bool self){
		return ::cinamo::toString(self);
	});
}

std::string BoolProvider::repr(const Object* ptr) const
{
	return BoolProvider::fromPointer(ptr) ? "bool: true" : "bool: false";
}

std::string BoolProvider::print(const Object* ptr) const
{
	return BoolProvider::fromPointer(ptr) ? "true" : "false";
}

bool BoolProvider::toBool(const Object* ptr) const
{
	return BoolProvider::fromPointer(ptr);
}

Handler<Object> BoolProvider::create( bool const& val )
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( BoolProvider::toPointer(val) );
}

}
