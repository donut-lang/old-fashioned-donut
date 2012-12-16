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

#include "FrameCombo.h"
#include "ElementFactory.h"
#include "../../util/XMLAttrParser.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(FrameCombo)
{
}

FrameCombo::~FrameCombo() noexcept
{

}

std::size_t FrameCombo::bringToFront(Handler<Element> const& e)
{
	auto it = std::find(elements_.begin(), elements_.end(), e);
	std::size_t s = std::distance(elements_.begin(), it);
	if(it == this->elements_.end()){
		throw logging::Exception(__FILE__, __LINE__, "Element: %s is not contained in this combo.", e->toString().c_str());
	}
	elements_.erase(it);
	elements_.push_back(e);
	this->layout(this->screenArea().box());
	return s;
}

Handler<Element> FrameCombo::getChildAt(const std::size_t index) const
{
	return elements_.at(index);
}

std::size_t FrameCombo::getChildCount() const
{
	return elements_.size();
}

std::string FrameCombo::toString() const
{
	return util::format("(FrameCombo %p with %d elements)", this, getChildCount());
}

void FrameCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		elm->render(	canvas,screenArea,area);
	}
}

geom::Box FrameCombo::onMeasure(geom::Box const& constraint)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		return elm->measure( constraint );
	}
	return constraint;
}

void FrameCombo::onLayout(geom::Box const& size)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		elm->layout( size );
	}
}

void FrameCombo::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	for( tinyxml2::XMLElement* e = element->FirstChildElement(); e; e=e->NextSiblingElement() ){
		this->elements_.push_back( factory->parseTree(this->self(), e) );
	}
}

Handler<Element> FrameCombo::getElementByIdImpl(std::string const& id)
{
	for( Handler<Element>& e : this->elements_ ){
		if(Handler<Element> w = e->getElementById(id)){
			return w;
		}
	}
	return Handler<Element>();
}

}}}
