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
#include "ElementObject.h"
#include "WorldObject.h"

namespace donut {
namespace native {

template <>
Handler<Object> encode< ::tarte::Handler<chisa::tk::Element> >(Handler<Heap> const& heap, ::tarte::Handler<chisa::tk::Element> val)
{
	return val->donutObject();
}

}}

namespace chisa {
namespace tk {

const static std::string TAG("ElementObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

ElementProvider::ElementProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world )
:ReactiveProvider(heap, name)
,world_(world)
{
	this->registerPureNativeClosure("findElementById", &ElementObject::findElementById);
	this->registerPureNativeClosure("findRootElement", &ElementObject::findRootElement);
}

Handler<World> ElementProvider::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

//---------------------------------------------------------

ElementObject::ElementObject(ElementProvider* provider)
:ReactiveNativeObject(provider)
,world_(provider->world())
{
}

Handler<World> ElementObject::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

Handler<Element> ElementObject::findRootElement()
{
	return this->element()->findRootElement();
}

Handler<Object> ElementObject::findElementById(std::string const& id)
{
	Handler<Element> found (this->element()->findElementById(id));
	if( found ) {
		return found->donutObject();
	}else{
		this->provider()->heap().lock()->createNull();
	}
}


}}
