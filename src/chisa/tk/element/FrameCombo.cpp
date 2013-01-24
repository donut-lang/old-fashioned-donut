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

#include <tarte/XMLAttrParser.h>

#include "FrameCombo.h"
#include "ElementFactory.h"

namespace chisa {
namespace tk {

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(FrameCombo, Super)
,numShown_(0)
{
}

FrameCombo::~FrameCombo() noexcept
{

}

std::string FrameCombo::toString() const
{
	return ::tarte::format("(FrameCombo %p with %d elements)", this, getChildCount());
}

void FrameCombo::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	for(std::vector<std::pair<Handler<Element>, bool> >::const_reverse_iterator it = std::vector<std::pair<Handler<Element>, bool> >::const_reverse_iterator(this->children().cbegin()+numShown_); it != children().crend(); ++it) {
		std::pair<Handler<Element>, bool> pair = *it;
		pair.first->render(canvas,ptInRoot,mask);
	}
}

geom::Box FrameCombo::measureImpl(geom::Box const& constraint)
{
	geom::Box size;
	this->numShown_ = 0;
	for(std::pair<Handler<Element>, bool> pair : this->children()) {
		size = geom::max(pair.first->measure( constraint ), size);
		++numShown_;
		if( !pair.second ) {
			break;
		}
	}
	return size.isSpecified() ? size : constraint;
}

void FrameCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	this->numShown_ = 0;
	for(std::pair<Handler<Element>, bool> pair : this->children()) {
		pair.first->layout(offsetFromParent, size);
		++numShown_;
		if( !pair.second ) {
			break;
		}
	}
}

void FrameCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	for( tinyxml2::XMLElement* e = element->FirstChildElement(); e; e=e->NextSiblingElement() ){
		bool transparent;
		::tarte::xml::parseAttr("transparent", transparent, false, e);
		this->addChild( factory->parseTree(this->self(), e), transparent);
	}
}

}}
