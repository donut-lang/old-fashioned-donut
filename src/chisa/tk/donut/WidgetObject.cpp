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

#include <functional>
#include "WidgetObject.h"
#include "WorldObject.h"
#include "../element/WidgetElement.h"

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
