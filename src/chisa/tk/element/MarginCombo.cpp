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

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(MarginCombo, Super)
{
	this->margin_ = geom::Space(0,0,0,0);
	this->addAttribute("margin", this->margin_);
}

MarginCombo::~MarginCombo() noexcept
{

}

void MarginCombo::addChild(Handler<Element> const& h, Context const& ctx)
{
	if(getChildCount() > 0){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Margin Combo can hold only one element.");
	}
	this->Super::addChild(h,ctx);
}
void MarginCombo::addChild(std::size_t const& idx, Handler<Element> const& h, Context const& ctx)
{
	if(getChildCount() > 0){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Margin Combo can hold only one element.");
	}
	this->Super::addChild(idx,h,ctx);
}

std::string MarginCombo::toString() const
{
	return util::format("(MarginCombo %p)", this);
}

void MarginCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	if(auto c = this->frontChild()){
		c->render(
			canvas,
			geom::Area(screenArea.point()+this->margin_.offset(), screenArea.box()-this->margin_.totalSpace()),
			geom::Area(area.point(), area.box()-this->margin_.totalSpace())
		);
	}
}

geom::Box MarginCombo::measureImpl(geom::Box const& constraint)
{
	if(auto c = this->frontChild()){
		return c->measure( constraint-this->margin_.totalSpace() ) + this->margin_.totalSpace();
	}
	return constraint;
}

void MarginCombo::layoutImpl(geom::Box const& size)
{
	if(auto c = this->frontChild()){
		c->layout( size-this->margin_.totalSpace() );
	}
}

void MarginCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	this->addChild( factory->parseTree(this->self(), element->FirstChildElement()) );
}


}}
