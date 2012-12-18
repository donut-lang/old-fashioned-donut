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

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(FrameCombo, Super)
{
}

FrameCombo::~FrameCombo() noexcept
{

}

std::string FrameCombo::toString() const
{
	return util::format("(FrameCombo %p with %d elements)", this, getChildCount());
}

void FrameCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if( Handler<Element> elm = this->lastChild() ){
		elm->render(	canvas,screenArea,area);
	}
}

geom::Box FrameCombo::measureImpl(geom::Box const& constraint)
{
	if( Handler<Element> elm = this->lastChild() ){
		return elm->measure( constraint );
	}
	return constraint;
}

void FrameCombo::layoutImpl(geom::Box const& size)
{
	if( Handler<Element> elm = this->lastChild() ){
		elm->layout( size );
	}
}

void FrameCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	for( tinyxml2::XMLElement* e = element->FirstChildElement(); e; e=e->NextSiblingElement() ){
		this->addChild( factory->parseTree(this->self(), e) );
	}
}

}}
