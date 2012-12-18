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

Element::Element(logging::Logger& log, HandlerW<World> world, HandlerW<Element> parent)
:log_(log)
,world_(world)
,parent_(parent)
,dirty_(false)
{
	this->addAttribute("id", this->id_);
}

void Element::idle(const float delta_ms)
{
}

void Element::loadXml(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	for(auto it : this->attrMap_){
		//firstには要素名が、secondにはnodeをパースして設定する関数オブジェクトが入ってる
		it.second(element);
	}
	this->loadXmlImpl(factory, element);
}

Handler<Element> Element::findRootElement()
{
	Handler<Element> root(self());
	for(Handler<Element> element=parent().lock(); element; element=element->parent().lock()){
		root=element;
	}
	return root;
}

Handler<Element> Element::findElementById(std::string const& id)
{
	return id == this->id() ? this->self() : Handler<Element>();
}

Handler<Element> Element::findElementByPoint(geom::Vector const& screenPoint)
{
	return this->screenArea().contain(screenPoint) ? this->self() : Handler<Element>();
}


std::string Element::toString() const
{
	return util::format("(Element %p)", this);
}

geom::Box Element::measure(geom::Box const& constraint)
{
	return this->measureImpl(constraint);
}

void Element::render(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( this->dirty_ ){
		this->revalidate();
	}
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

void Element::layout(geom::Box const& size)
{
	this->layoutImpl(size);
	this->size(size);
}

bool Element::isValidationRoot() const noexcept
{
	return false;
}

void Element::invalidate()
{
	this->dirty_ = true;
	if(Handler<Element> p = this->parent().lock()) {
		if(!p->isValidationRoot()){
			p->invalidate();
		}
	}
}

void Element::revalidate()
{
	this->measure(this->size_);
	this->layout(this->size_);
	this->validate();
}

void Element::validate()
{
	this->dirty_= false;
}

}}
