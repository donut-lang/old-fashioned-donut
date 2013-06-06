/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <functional>
#include "GeistObject.h"
#include "WorldObject.h"

namespace donut {
namespace native {

template <>
Handler<Object> encode< ::cinamo::Handler<chisa::WorldGeist> >(Handler<Heap> const& heap, ::cinamo::Handler<chisa::WorldGeist> val)
{
	return val->donutObject(heap);
}

}}

namespace chisa {
namespace tk {

const static std::string TAG("GeistObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

GeistProvider::GeistProvider( Handler<Heap> const& heap, std::string const& name )
:ReactiveProvider(heap, name)
{
}

//---------------------------------------------------------

GeistObject::GeistObject(GeistProvider* provider)
:ReactiveNativeObject(provider)
{
}


Handler<WorldGeist> GeistObject::geist()
{
	return this->geist_.lock();
}

}}
