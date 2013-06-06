/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/XMLAttrParser.h>
#include "MarginCombo.h"
#include "ElementFactory.h"

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
		CINAMO_EXCEPTION(Exception, "[BUG] Margin Combo can hold only one element.");
	}
	this->Super::addChild(h,ctx);
}
void MarginCombo::addChild(std::size_t const& idx, Handler<Element> const& h, Context const& ctx)
{
	if(getChildCount() > 0){
		CINAMO_EXCEPTION(Exception, "[BUG] Margin Combo can hold only one element.");
	}
	this->Super::addChild(idx,h,ctx);
}

std::string MarginCombo::toString() const
{
	return ::cinamo::format("(MarginCombo %p)", this);
}

void MarginCombo::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	geom::Area newMask(margin_.apply(mask));
	newMask.point() -= margin_.offset();
	if(auto c = this->frontChild()){
		c->render(
			canvas,
			ptInRoot+this->margin_.offset(),
			newMask
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

void MarginCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	if(auto c = this->frontChild()){
		c->layout( offsetFromParent+this->margin_.offset(), size-this->margin_.totalSpace() );
	}
}

void MarginCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	this->addChild( factory->parseTree(this->self(), element->FirstChildElement()) );
}


}}
