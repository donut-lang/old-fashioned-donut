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

#include "Hexe.h"
#include "tk/World.h"

namespace chisa {

Hexe::Hexe(Logger& log, std::string const& basepath)
:log_(log)
,basepath_(basepath)
{
}

Hexe::~Hexe() noexcept
{
}

std::string Hexe::toString() const
{
	return ::cinamo::format("(Hexe %p)", this);
}

WorldGeist::WorldGeist(Logger& log, Handler<Hexe> const& hexe, HandlerW<chisa::tk::World> world)
:log_(log)
,hexe_(hexe)
,world_(world)
{

}
WorldGeist::~WorldGeist() noexcept
{

}

Handler< ::donut::Object> WorldGeist::donutObject(Handler< ::donut::Heap> const& heap)
{
	if( this->donutObject_.expired() ) {
		Handler< ::donut::Object> obj(createDonutObject(heap));
		this->donutObject_ = obj;
		return obj;
	}else{
		return this->donutObject_.lock();
	}
}

Handler<chisa::tk::World> WorldGeist::world()
{
	return world_.lock();
}

Handler<Hexe> WorldGeist::hexe()
{
	return hexe_.lock();
}
std::string WorldGeist::toString() const{
	return format("(WorldGeist %p)", this);
}


}
