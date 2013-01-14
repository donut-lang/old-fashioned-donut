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

#include "ElementServant.h"
#include "../Element.h"
#include "../World.h"
#include "../donut/Patron.h"
#include "../../gl/DecoUtil.h"

namespace chisa {
namespace tk {

ElementServant::ElementServant(Handler<AngelTarget> const& angelTarget, Handler<Element> const& elem)
:Servant(angelTarget)
,element_(elem)
,anim_(0)
{

}

Handler<ElementServant> ElementServant::self() noexcept
{
	return Handler<ElementServant>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> ElementServant::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->elementServantProvider()->newInstance(heap, self()) );
}

void ElementServant::renderImpl(gl::Canvas& canvas)
{
	geom::Box box = this->element_->size();
	geom::Area area = this->target()->findScreenArea();

	float offsetX = 0;
	float lineOffsetX = 0;
	float midX;
	if( area.right() + box.width() < renderArea().right() ) { //右側に描けるよ！
		lineOffsetX = offsetX = area.right();
		midX = box.width();
	}else if( area.left() - box.width() > 0 ){ //左なら開いてるよ！
		offsetX = area.left() - box.width();
		lineOffsetX = area.left();
		midX=0;
	}else{
		lineOffsetX = offsetX = 0;
		midX=0;
	}

	float offsetY = 0;
	float const midY = (area.top() + area.bottom())/2;
	if( area.bottom() + box.height() < renderArea().bottom() ) {
		offsetY = area.bottom();
	}else if( area.top() - box.height() > 0 ) {
		offsetY = area.top() - box.height();
	}else{
		offsetY = 0;
	}

	float const cycleY = std::sin( anim_/1000.0f ) * box.height() * 0.1f;
	float const cycleX = std::sin( anim_/1740.0f ) * box.width() * 0.1f;

	geom::Point pt(offsetX+cycleX, offsetY+cycleY);
	geom::Box leftSize( renderArea().width()-offsetX, renderArea().height()-offsetY );
	geom::Point lpt(offsetX+cycleX+midX, offsetY+cycleY);

	gl::renderCubicLine(canvas, 2, gl::Color(1.0f,.8f,.8f,.7f), geom::Point(lineOffsetX, midY), lpt);
	geom::Box maskSize(geom::min(box, leftSize));
	if(maskSize.width() > 0 && maskSize.height() > 0) {
		this->element()->render(canvas, pt, geom::Area(geom::ZERO, maskSize));
	}
}

void ElementServant::idleImpl(const float delta_ms)
{
	this->anim_ += delta_ms;
}

geom::Box ElementServant::reshapeImpl(const geom::Area& area)
{
	geom::Box requested(element()->measure(geom::Box()));
	geom::Box _size(
			geom::isUnspecified(requested.width()) || (area.box().width()/4 <= requested.width()) ? area.box().width()/4 : requested.width(),
			geom::isUnspecified(requested.height() || (area.box().height()/4 <= requested.height())) ? area.box().height()/4 : requested.height()
			);
	element()->layout(geom::ZERO, _size);
	return _size;
}

Handler<Element> ElementServant::findElementById(const std::string& id)
{
	return this->element()->findElementById(id);
}

Handler<Element> ElementServant::findElementByPoint(const geom::Point& screenVector)
{
	return this->element()->findElementByPoint(screenVector);
}

}}

