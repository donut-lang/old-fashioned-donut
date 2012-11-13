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

#include "Element.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {

Element::Element(logging::Logger& log, weak_ptr<World> world, weak_ptr<Element> root, weak_ptr<Element> parent)
:log_(log)
,world_(world)
,root_(root)
,parent_(parent)
{

}

void Element::init(weak_ptr<Element> _self)
{
	this->self_.swap(_self);
}

void Element::idle(const float delta_ms)
{
	const size_t max = this->getChildCount();
	for(size_t i=0;i<max;++i){
		if( shared_ptr<Element> child = this->getChildAt(i).lock()){
			child->idle(delta_ms);
		}
	}
}

void Element::loadXML(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{

	if(const char* id = element->Attribute("id", nullptr)){
		this->id(id);
	}
	this->loadXMLimpl(factory, element);
}

weak_ptr<Element> Element::getElementById(const std::string& id)
{
	return id == this->id() ? this->self() : this->getElementByIdImpl(id);
}

weak_ptr<Element> Element::getElementByPoint(const geom::Vector& screenPoint)
{
	if(!this->screenArea().contain(screenPoint)){
		return weak_ptr<Element>();
	}
	const size_t max = this->getChildCount();
	for(size_t i=0;i<max;++i){
		if(shared_ptr<Element> child = this->getChildAt(i).lock()){
			if(child->screenArea().contain(screenPoint)){
				return child->getElementByPoint(screenPoint);
			}
		}
	}
	return this->self();
}


string Element::toString() const
{
	return util::format("(Layout %p)", this);
}

geom::Box Element::measure(const geom::Box& constraint)
{
	return this->onMeasure(constraint);
}
void Element::render(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	this->screenArea(screenArea);
	this->drawnArea(area);
	if( //描画範囲にないので書く必要性がありません。
		area.height()+area.y() <= 0 ||
		area.width()+area.x() <= 0 ||
		area.x() >= this->size().width() ||
		area.y() >= this->size().height() ){
		return;
	}
	this->renderImpl(canvas, screenArea, area);
}

void Element::layout(const geom::Box& size)
{
	this->onLayout(size);
	this->size(size);
}


}}
