/*
 * ScrollLayout.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#include "ScrollLayout.h"

namespace chisa {
namespace tk {
namespace layout {


CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(ScrollLayout)
,scrollMode_(None)
{

}

ScrollLayout::~ScrollLayout()
{
}

weak_ptr<Layout> ScrollLayout::getChildAt(const size_t index) const
{
	if(index == 0 && this->child_){
		return this->child_;
	}else{
		return weak_ptr<Layout>();
	}
}

size_t ScrollLayout::getChildCount() const
{
	if(this->child_){
		return 1;
	}else{
		return 0;
	}
}

void ScrollLayout::loadXMLimpl(LayoutFactory* const factory, tinyxml2::XMLElement* element)
{
	if( const char* _mode = element->Attribute("mode", nullptr) ){
		std::string mode(_mode);
		if(mode == "both"){
			this->scrollMode_ = Both;
		}else if(mode == "horizontal"){
			this->scrollMode_ = Horizontal;
		}else if(mode == "vertical"){
			this->scrollMode_ = Vertical;
		}
	}
	XMLElement* const childElement = element->FirstChildElement();
	if(childElement) {
		this->child_ = factory->parseTree(root(), self(), childElement);
	}
}

string ScrollLayout::toString()
{
	return util::format("(ScrollLayout %p)", this);
}

void ScrollLayout::renderImpl(gl::Canvas& canvas, const Area& screenArea, const Area& area)
{
	const Area scrolledArea(this->nowPosition_, this->size());
	this->child_->render(canvas, screenArea, area.intersect(scrolledArea));
}

Box ScrollLayout::onMeasure(const Box& constraint)
{
	return Box(geom::VeryLarge, geom::VeryLarge);
}

void ScrollLayout::onLayout(const Box& size)
{
	if(this->child_){
		Box childBox(this->scrollMode_ & Horizontal ? geom::Unspecified : size.width(), this->scrollMode_ & Vertical ? geom::Unspecified : size.height());
		this->childSize_ = this->child_->measure(childBox);
		this->child_->layout(this->childSize_);
	}
}

weak_ptr<Layout> ScrollLayout::getLayoutByIdImpl(const std::string& id)
{
	if(this->child_){
		return this->child_->getLayoutById(id);
	}else{
		return weak_ptr<Layout>();
	}
}

}}}
