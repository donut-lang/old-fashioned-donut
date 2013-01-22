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
	for(std::pair<Handler<Element>, bool> pair : this->children()) {
		pair.first->render(canvas,ptInRoot,mask);
		if( !pair.second ) {
			break;
		}
	}
}

geom::Box FrameCombo::measureImpl(geom::Box const& constraint)
{
	geom::Box size;
	for(std::pair<Handler<Element>, bool> pair : this->children()) {
		size = geom::max(pair.first->measure( constraint ), size);
		if( !pair.second ) {
			break;
		}
	}
	return size.isSpecified() ? size : constraint;
}

void FrameCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	for(std::pair<Handler<Element>, bool> pair : this->children()) {
		pair.first->layout(offsetFromParent, size);
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
