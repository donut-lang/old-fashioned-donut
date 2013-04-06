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
