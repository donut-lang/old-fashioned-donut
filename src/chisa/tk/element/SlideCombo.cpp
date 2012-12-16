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

#include "SlideCombo.h"
#include "ElementFactory.h"
#include "../../util/XMLAttrParser.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(SlideCombo)
{
}

SlideCombo::~SlideCombo() noexcept
{

}

void SlideCombo::bringToFront(Handler<Element>& e)
{
	auto it = std::find(elements_.begin(), elements_.end(), e);
	if(it == this->elements_.end()){
		throw logging::Exception(__FILE__, __LINE__, "Element: %s is not contained in this combo.", e->toString().c_str());
	}
	elements_.erase(it, elements_.end());
	elements_.push_back(e);
}

HandlerW<Element> SlideCombo::getChildAt(const std::size_t index) const
{
	return elements_.at(index);
}

std::size_t SlideCombo::getChildCount() const
{
	return elements_.size();
}

std::string SlideCombo::toString() const
{
	return util::format("(SlideCombo %p with %d elements)", this, getChildCount());
}

void SlideCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		elm->render(	canvas,screenArea,area);
	}
}

geom::Box SlideCombo::onMeasure(geom::Box const& constraint)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		return elm->measure( constraint );
	}
	return constraint;
}

void SlideCombo::onLayout(geom::Box const& size)
{
	if( this->getChildCount() > 0 ){
		Handler<Element>& elm = this->elements_.back();
		elm->layout( size );
	}
}

void SlideCombo::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	factory->parseTree(this->root(), this->self(), element->FirstChildElement());
}

HandlerW<Element> SlideCombo::getElementByIdImpl(std::string const& id)
{
	for( Handler<Element>& e : this->elements_ ){
		HandlerW<Element> w = e->getElementById(id);
		if(!w.expired()){
			return w;
		}
	}
	return HandlerW<Element>();
}

}}}
