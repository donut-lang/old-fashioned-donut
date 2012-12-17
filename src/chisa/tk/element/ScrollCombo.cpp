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
#include "ScrollCombo.h"
#include "../World.h"

namespace chisa {
namespace tk {
namespace element {


CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ScrollCombo, ElementGroup)
,scrollMode_(None)
,scrollOffset_(0,0)
,lastMovedFrom_(0)
{

}

ScrollCombo::~ScrollCombo() noexcept
{
}

void ScrollCombo::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* element)
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
	tinyxml2::XMLElement* const childElement = element->FirstChildElement();
	if(childElement) {
		this->child_ = factory->parseTree(this->self(), childElement);
	}
}

std::string ScrollCombo::toString() const
{
	return util::format("(ScrollCombo %s %p)",
					(this->scrollMode_ == Vertical) ? "vertical":
					(this->scrollMode_ == Horizontal) ? "horizontal":
					(this->scrollMode_ == Both) ? "both" : "???",
					this);
}

void ScrollCombo::renderImpl(gl::Canvas& canvas, geom::Area const& screenArea, geom::Area const& area)
{
	const geom::Area clipArea(this->scrollOffset_, this->size());
	const geom::Area logicalArea(area.point()+this->scrollOffset_, area.box());
	this->child_->render(canvas, screenArea, clipArea.intersect(logicalArea));
	if(this->lastMovedFrom_ < ScrollBarTimeOut){
		const float alpha = (ScrollBarTimeOut - this->lastMovedFrom_) / ScrollBarTimeOut;
		if((this->scrollMode_ & Vertical)){
			const float scrollMax = area.height() - this->childSize_.height();
			const float scrollRatio = this->scrollOffset_.y() / scrollMax;
			const float x = screenArea.x()+screenArea.width()-7.0f;
			const float len = (screenArea.height()-4.0f) * area.height() / this->childSize_.height();
			const float y = screenArea.y()+2.0f - (screenArea.height()-len-4.0f) * scrollRatio;
			canvas.drawLine(5.0f, gl::Color(.5f, .5f, .5f, alpha), geom::Point(x, std::max(y, screenArea.y())), geom::Point(x, std::min(y+len, screenArea.y()+screenArea.height())), 1.0f);
		}
		if((this->scrollMode_ & Horizontal)){
			const float scrollMax = area.width() - this->childSize_.width();
			const float scrollRatio = this->scrollOffset_.x() / scrollMax;

			const float y = screenArea.y()+screenArea.height()-7.0f;
			const float len = (screenArea.width()-4.0f) * area.width() / this->childSize_.width();
			const float x = screenArea.x()+2.0f - (screenArea.width()-len-4.0f) * scrollRatio;
			canvas.drawLine(5.0f, gl::Color(.5f, .5f, .5f, alpha), geom::Point(std::max(x, screenArea.x()), y), geom::Point(std::min(x+len, screenArea.x()+screenArea.width()), y), 1.0f);
		}
	}
}

geom::Box ScrollCombo::measureImpl(geom::Box const& constraint)
{
	return geom::Box(geom::Unspecified, geom::Unspecified);
}

void ScrollCombo::layoutImpl(geom::Box const& size)
{
	if(this->child_){
		geom::Box childBox((this->scrollMode_ & Horizontal) == Horizontal ? geom::Unspecified : size.width(), (this->scrollMode_ & Vertical) == Vertical ? geom::Unspecified : size.height());
		this->childSize_ = this->child_->measure(childBox);
		this->childSize_ = geom::Box(
				(geom::isSpecified(this->childSize_.width()) ? this->childSize_.width() : size.width()),
				(geom::isSpecified(this->childSize_.height()) ? this->childSize_.height() : size.height())
		);
		this->child_->layout(this->childSize_);
	}
}

void ScrollCombo::idle(const float delta_ms)
{
	geom::Point ptStart(this->scrollOffset_);
	geom::Point ptEnd(this->scrollOffset_+this->size());
	if((this->scrollMode_ & Horizontal) && ptStart.x() < 0){
		this->scrollOffset_.x(this->scrollOffset_.x() - (ptStart.x()*delta_ms/100));
	}
	if((this->scrollMode_ & Vertical) && ptStart.y() < 0){
			this->scrollOffset_.y(this->scrollOffset_.y() - (ptStart.y()*delta_ms/100));
	}
	if((this->scrollMode_ & Horizontal) && ptEnd.x() > this->childSize_.width()){
		this->scrollOffset_.x(this->scrollOffset_.x() - ((ptEnd.x() - this->childSize_.width()) * delta_ms/100));
	}
	if((this->scrollMode_ & Vertical) && ptEnd.y() > this->childSize_.height()){
		this->scrollOffset_.y(this->scrollOffset_.y() - ((ptEnd.y() - this->childSize_.height()) * delta_ms/100));
	}
	this->lastMovedFrom_ += delta_ms;
}

bool ScrollCombo::onScroll(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance)
{
	if(this->scrollMode_ == Both){
		this->scrollOffset_ -= distance;
	}else if(this->scrollMode_ == Vertical){
		this->scrollOffset_.y(this->scrollOffset_.y() - distance.y());
	}else if(this->scrollMode_ == Horizontal){
		this->scrollOffset_.x(this->scrollOffset_.x() - distance.x());
	}
	this->lastMovedFrom_ = 0.0f;
	return true; //イベントを消費する
}



}}}
