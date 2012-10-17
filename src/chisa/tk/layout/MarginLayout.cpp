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

#include "MarginLayout.h"

namespace chisa {
namespace tk {
namespace layout {

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(MarginLayout)
{

}

MarginLayout::~MarginLayout()
{

}

weak_ptr<Layout> MarginLayout::getChildAt(const size_t index) const
{
	return index == 0 ? this->child_ : weak_ptr<Layout>();
}

size_t MarginLayout::getChildCount() const
{
	return this->child_.expired() ? 0 : 1;
}

string MarginLayout::toString() const
{
	return util::format("(MarginLayout %p)", this);
}

void MarginLayout::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	if( shared_ptr<Layout> child = this->child_.lock() ){
		child->render(
			canvas,
			geom::Area(screenArea.point()+this->margin_, screenArea.box()-this->removedSize_),
			geom::Area(area.point()+this->margin_, area.box()-this->removedSize_)
		);
	}
}

geom::Box MarginLayout::onMeasure(const geom::Box& constraint)
{
	if( shared_ptr<Layout> child = this->child_.lock() ){
		return child->measure( constraint-this->removedSize_ );
	}
	return constraint;
}

void MarginLayout::onLayout(const geom::Box& size)
{
	if( shared_ptr<Layout> child = this->child_.lock() ){
		child->layout( size-this->removedSize_ );
	}
}

void MarginLayout::loadXMLimpl(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element)
{
	float v;
	float marginTop = 0.0f;
	float marginRight = 0.0f;
	float marginLeft = 0.0f;
	float marginBottom = 0.0f;
	if(element->QueryFloatAttribute("margin", &v)){
		marginTop = v;
		marginLeft = v;
		marginRight = v;
		marginBottom = v;
	}
	element->QueryFloatAttribute("margin-top", &marginTop);
	element->QueryFloatAttribute("margin-left", &marginLeft);
	element->QueryFloatAttribute("margin-right", &marginRight);
	element->QueryFloatAttribute("margin-bottom", &marginBottom);

	this->margin_.x(marginLeft);
	this->margin_.y(marginTop);
	this->removedSize_.x(marginTop+marginBottom);
	this->removedSize_.y(marginLeft+marginRight);

	factory->parseTree(this->root(), this->self(), element->FirstChildElement());
}

weak_ptr<Layout> MarginLayout::getLayoutByIdImpl(const std::string& id)
{
	if( shared_ptr<Layout> child = this->child_.lock() ){
		return child->getLayoutById(id);
	}
	return weak_ptr<Layout>();
}

}}}
