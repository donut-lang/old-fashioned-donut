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

namespace chisa {
namespace tk {

const static std::string TAG("ElementObject");

ElementProvider::ElementProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world )
:HeapProvider(heap, name)
,world_(world)
{
}

Handler<World> ElementProvider::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

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

}}
