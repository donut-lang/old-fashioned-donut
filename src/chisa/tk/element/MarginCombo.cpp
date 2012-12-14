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

#include "MarginCombo.h"
#include "ElementFactory.h"
#include "../../util/XMLAttrParser.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(MarginCombo)
{
	this->margin_ = geom::Space(0,0,0,0);
	this->addAttribute("margin", this->margin_);
}

MarginCombo::~MarginCombo() noexcept
{

}

HandlerW<Element> MarginCombo::getChildAt(const std::size_t index) const
{
	return index == 0 ? this->child_ : HandlerW<Element>();
}

std::size_t MarginCombo::getChildCount() const
{
	return this->child_.expired() ? 0 : 1;
}

std::string MarginCombo::toString() const
{
	return util::format("(MarginCombo %p)", this);
}

void MarginCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( Handler<Element> child = this->child_.lock() ){
		child->render(
			canvas,
			geom::Area(screenArea.point()+this->margin_.offset(), screenArea.box()-this->margin_.totalSpace()),
			geom::Area(area.point(), area.box()-this->margin_.totalSpace())
		);
	}
}

geom::Box MarginCombo::onMeasure(geom::Box const& constraint)
{
	if( Handler<Element> child = this->child_.lock() ){
		return child->measure( constraint-this->margin_.totalSpace() ) + this->margin_.totalSpace();
	}
	return constraint;
}

void MarginCombo::onLayout(geom::Box const& size)
{
	if( Handler<Element> child = this->child_.lock() ){
		child->layout( size-this->margin_.totalSpace() );
	}
}

void MarginCombo::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	factory->parseTree(this->root(), this->self(), element->FirstChildElement());
}

HandlerW<Element> MarginCombo::getElementByIdImpl(std::string const& id)
{
	if( Handler<Element> child = this->child_.lock() ){
		return child->getElementById(id);
	}
	return HandlerW<Element>();
}

}}}
