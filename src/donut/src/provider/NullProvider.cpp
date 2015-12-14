/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/Exception.hpp>
#include <donut/provider/NullProvider.hpp>
#include <donut/object/Heap.hpp>

namespace donut {

static const std::string TAG("NullProvider");

NullProvider::NullProvider(Handler<Heap> const& heap)
:Provider(heap, "Null")
{
	this->registerPureNativeClosure("opEq", [&](Object* self, Object* obj) {
		return self->isNull() && obj->isNull();
	});
	this->registerPureNativeClosure("opNe", [&](Object* self, Object* obj) {
		return !self->isNull() || !obj->isNull();
	});
}

std::string NullProvider::repr(const Object* ptr) const
{
	return std::string("null");
}

std::string NullProvider::print(const Object* ptr) const
{
	return std::string("");
}

Handler<Object> NullProvider::createNull()
{
	return Handler<Object>::__internal__fromRawPointerWithoutCheck( NullProvider::toPointer() );
}

}
