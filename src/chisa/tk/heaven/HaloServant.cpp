/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "HaloServant.hpp"
#include "../Element.hpp"
#include "../World.hpp"
#include "../donut/Patron.hpp"

namespace chisa {
namespace tk {

HaloServant::HaloServant(Handler<AngelTarget> const& angelTarget, gl::Color const& color)
:Servant(angelTarget)
,color_(color)
{

}

Handler<HaloServant> HaloServant::self() noexcept
{
	return Handler<HaloServant>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> HaloServant::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->haloServantProvider()->newInstance(heap, self()) );
}

void HaloServant::renderImpl(gl::Canvas& canvas)
{
	canvas.drawRect(2, this->color_, this->target()->findScreenArea());
}

void HaloServant::idleImpl(const float delta_ms)
{
}

geom::Box HaloServant::reshapeImpl(const geom::Area& area)
{
	return this->target()->findScreenArea().box();
}

}}
