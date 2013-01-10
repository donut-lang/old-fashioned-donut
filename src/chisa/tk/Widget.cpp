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

#include "Widget.h"
#include "element/WidgetElement.h"
#include <GL/gl.h>

namespace chisa {
namespace tk {

const static std::string TAG("Widget");

Widget::Widget(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element)
:log_(log)
,world_(world)
{
}

void Widget::render(gl::Canvas& cv, geom::Area const& area)
{
	this->renderImpl(cv, area);
	this->lastDrawnArea_ = area;
}
void Widget::idle(const float delta_ms)
{
	this->idleImpl(delta_ms);
}
void Widget::reshape(geom::Box const& areaSize)
{
	this->reshapeImpl(areaSize);
	this->size_ = areaSize;
}
geom::Box Widget::measure(geom::Box const& constraintSize)
{
	return this->measureImpl(constraintSize);
}


geom::Area Widget::findTarget(std::string const& target)
{
	return this->findTargetImpl(target);
}
geom::Area Widget::findTargetInRoot(std::string const& target)
{
	Handler<WidgetElement> const elm = this->wrapper_.lock();
	if(!elm){
		return geom::Area();
	}
	return elm->calcAreaInRoot( findTarget(target) );
}
geom::Area Widget::findTargetInElement(std::string const& target)
{
	Handler<WidgetElement> const elm = this->wrapper_.lock();
	if(!elm){
		return geom::Area();
	}
	return elm->calcAreaInRoot( findTarget(target) );
}

void Widget::requestRelayout()
{
	if(Handler<WidgetElement> elm = this->wrapper_.lock()){
		elm->requestRelayout();
	}
}

void Widget::notifyRelayoutFinished()
{
	this->notifyRelayoutFinishedImpl();
}

void Widget::notifyViewRefreshed()
{
	if(Handler<WidgetElement> elm = this->wrapper_.lock()){
		elm->notifyViewRefreshed();
	}
}

}}
