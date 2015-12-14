/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Hexe.hpp"
#include "tk/World.hpp"

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
