/*
 * ScrollLayout.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#include "ScrollLayout.h"
#include "../World.h"

namespace chisa {
namespace tk {
namespace layout {


CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(ScrollLayout)
,scrollMode_(None)
,scrollDist_(0,0)
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
	const Area clipArea(this->scrollDist_, this->size());
	const Area logicalArea(area.point()+this->scrollDist_, area.box());
	this->child_->render(canvas, screenArea, clipArea.intersect(logicalArea));
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

void ScrollLayout::idle(const float delta_ms)
{
	Point ptStart(this->scrollDist_);
	Point ptEnd(ptStart.x()+this->size().width(), ptStart.y()+this->size().height());
	if(ptStart.x() < 0){
		this->scrollDist_.x(this->scrollDist_.x() - (ptStart.x()*delta_ms/100));
	}
	if(ptStart.y() < 0){
			this->scrollDist_.y(this->scrollDist_.y() - (ptStart.y()*delta_ms/100));
	}
	if(ptEnd.x() > this->childSize_.width()){
		this->scrollDist_.x(this->scrollDist_.x() - ((ptEnd.x() - this->childSize_.width()) * delta_ms/100));
	}
	if(ptEnd.y() > this->childSize_.height()){
		this->scrollDist_.y(this->scrollDist_.y() - ((ptEnd.y() - this->childSize_.height()) * delta_ms/100));
	}
}

bool ScrollLayout::onScroll(const Point& start, const Point& end, const Distance& distance)
{
	if(this->scrollMode_ == Both){
		this->scrollDist_ -= distance;
	}else if(this->scrollMode_ == Vertical){
		this->scrollDist_.x(this->scrollDist_.x() - distance.x());
	}else if(this->scrollMode_ == Horizontal){
		this->scrollDist_.y(this->scrollDist_.y() - distance.y());
	}
	return true; //イベントを消費する
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
