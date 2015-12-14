/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <functional>
#include "WidgetObject.hpp"
#include "WorldObject.hpp"
#include "../element/WidgetElement.hpp"

namespace donut {
namespace native {

template <>
Handler<Object> encode< ::cinamo::Handler<chisa::tk::Widget> >(Handler<Heap> const& heap, ::cinamo::Handler<chisa::tk::Widget> val)
{
	return val ? Handler<Object>(val->donutObject()) : heap->createNull();
}

}}
namespace chisa {
namespace tk {

const static std::string TAG("WidgetObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

WidgetProvider::WidgetProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world )
:ReactiveProvider(heap, name)
,world_(world)
{
}

Handler<World> WidgetProvider::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

//---------------------------------------------------------

WidgetObject::WidgetObject(WidgetProvider* provider)
:ReactiveNativeObject(provider)
,world_(provider->world())
{
}

Handler<World> WidgetObject::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

void WidgetObject::bootstrap(const Handler< ::donut::Heap>& heap, const Handler<Widget>& widget)
{
	this->ReactiveNativeObject::bootstrap(heap);
	const_cast<Handler<Widget>&>(widget_) = widget;
}


}}
