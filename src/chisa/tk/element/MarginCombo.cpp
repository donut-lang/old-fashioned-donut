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
#include "../../util/XMLUtil.h"

namespace chisa {
namespace tk {
namespace element {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF(MarginCombo)
{

}

MarginCombo::~MarginCombo() noexcept
{

}

weak_ptr<Element> MarginCombo::getChildAt(const size_t index) const
{
	return index == 0 ? this->child_ : weak_ptr<Element>();
}

size_t MarginCombo::getChildCount() const
{
	return this->child_.expired() ? 0 : 1;
}

string MarginCombo::toString() const
{
	return util::format("(MarginCombo %p)", this);
}

void MarginCombo::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	if( shared_ptr<Element> child = this->child_.lock() ){
		child->render(
			canvas,
			geom::Area(screenArea.point()+this->margin_.offset(), screenArea.box()-this->margin_.totalSpace()),
			geom::Area(area.point(), area.box()-this->margin_.totalSpace())
		);
	}
}

geom::Box MarginCombo::onMeasure(const geom::Box& constraint)
{
	if( shared_ptr<Element> child = this->child_.lock() ){
		return child->measure( constraint-this->margin_.totalSpace() ) + this->margin_.totalSpace();
	}
	return constraint;
}

void MarginCombo::onLayout(const geom::Box& size)
{
	if( shared_ptr<Element> child = this->child_.lock() ){
		child->layout( size-this->margin_.totalSpace() );
	}
}

void MarginCombo::loadXMLimpl(element::ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	chisa::util::xml::parseAttr("margin", this->margin_, geom::Margin(0,0,0,0), element);

	factory->parseTree(this->root(), this->self(), element->FirstChildElement());
}

weak_ptr<Element> MarginCombo::getElementByIdImpl(const std::string& id)
{
	if( shared_ptr<Element> child = this->child_.lock() ){
		return child->getElementById(id);
	}
	return weak_ptr<Element>();
}

}}}
