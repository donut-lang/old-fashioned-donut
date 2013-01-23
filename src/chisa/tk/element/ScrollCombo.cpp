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

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(ScrollCombo, Super)
,scrollMode_(None)
,scrollOffset_(0,0)
,lastMovedFrom_(0)
,pushedCnt_(0)
{

}

ScrollCombo::~ScrollCombo() noexcept
{
}


void ScrollCombo::addChild(Handler<Element> const& h, Context const& ctx)
{
	if(getChildCount() > 0){
		TARTE_EXCEPTION(Exception, "[BUG] Scroll Combo can hold only one element.");
	}
	this->Super::addChild(h,ctx);
}
void ScrollCombo::addChild(std::size_t const& idx, Handler<Element> const& h, Context const& ctx)
{
	if(getChildCount() > 0){
		TARTE_EXCEPTION(Exception, "[BUG] Scroll Combo can hold only one element.");
	}
	this->Super::addChild(idx,h,ctx);
}

bool ScrollCombo::isValidationRoot() const noexcept
{
	return true;
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
		this->addChild(factory->parseTree(this->self(), childElement));
	}
}

std::string ScrollCombo::toString() const
{
	return ::tarte::format("(ScrollCombo %s %p)",
					(this->scrollMode_ == Vertical) ? "vertical":
					(this->scrollMode_ == Horizontal) ? "horizontal":
					(this->scrollMode_ == Both) ? "both" : "???",
					this);
}

void ScrollCombo::renderImpl(gl::Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask)
{
	const geom::Area logicalArea(mask.point()+this->scrollOffset_, mask.box());
	this->frontChild()->render(canvas, ptInRoot-this->scrollOffset_, logicalArea);
	if(this->lastMovedFrom_ < ScrollBarTimeOut){
		const float alpha = (ScrollBarTimeOut - this->lastMovedFrom_) / ScrollBarTimeOut;
		if((this->scrollMode_ & Vertical)){
			const float scrollMax = innerSize().height() - this->childSize_.height();
			const float scrollRatio = this->scrollOffset_.y() / scrollMax;
			const float x = ptInRoot.x()+innerSize().width()-7.0f;
			const float len = (innerSize().height()-4.0f) * innerSize().height() / this->childSize_.height();
			const float y = ptInRoot.y()+2.0f - (innerSize().height()-len-4.0f) * scrollRatio;
			canvas.drawLine(5.0f, gl::Color(.5f, .5f, .5f, alpha), geom::Point(x, std::max(y, ptInRoot.y())), geom::Point(x, std::min(y+len, ptInRoot.y()+innerSize().height())), 1.0f);
		}
		if((this->scrollMode_ & Horizontal)){
			const float scrollMax = innerSize().width() - this->childSize_.width();
			const float scrollRatio = this->scrollOffset_.x() / scrollMax;

			const float y = ptInRoot.y()+innerSize().height()-7.0f;
			const float len = (innerSize().width()-4.0f) * innerSize().width() / this->childSize_.width();
			const float x = ptInRoot.x()+2.0f - (innerSize().width()-len-4.0f) * scrollRatio;
			canvas.drawLine(5.0f, gl::Color(.5f, .5f, .5f, alpha), geom::Point(std::max(x, ptInRoot.x()), y), geom::Point(std::min(x+len, ptInRoot.x()+innerSize().width()), y), 1.0f);
		}
	}
}

geom::Box ScrollCombo::measureImpl(geom::Box const& constraint)
{
	return geom::Box(geom::Unspecified, geom::Unspecified);
}

void ScrollCombo::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	if(auto child = this->frontChild()){
		geom::Box childBox((this->scrollMode_ & Horizontal) == Horizontal ? geom::Unspecified : size.width(), (this->scrollMode_ & Vertical) == Vertical ? geom::Unspecified : size.height());
		this->childSize_ = child->measure(childBox);
		this->childSize_ = geom::Box(
				(geom::isSpecified(this->childSize_.width()) ? this->childSize_.width() : size.width()),
				(geom::isSpecified(this->childSize_.height()) ? this->childSize_.height() : size.height())
		);
		child->layout(geom::ZERO, this->childSize_);
	}
}

void ScrollCombo::idle(const float delta_ms)
{
	Super::idle(delta_ms);
	if(this->pushedCnt_ == 0){
		geom::Point ptStart(this->scrollOffset_);
		geom::Point ptEnd(this->scrollOffset_+this->innerSize());
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
}

bool ScrollCombo::onDownRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	++this->pushedCnt_;
	return false;
}
bool ScrollCombo::onUpRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	--this->pushedCnt_;
	return false;
}

void ScrollCombo::showPointInner(const geom::Point& relPtFromParent)
{
	if(this->scrollMode_ & Vertical) {
		this->scrollOffset_.y(geom::min(childSize_.height()-lastDrawnAreaInRoot().height(),geom::max(0, relPtFromParent.y() - lastInnerDrawnAreaInRoot().height()/2)));
	}
	if(this->scrollMode_ & Horizontal){
		this->scrollOffset_.x(geom::min(childSize_.width()-lastDrawnAreaInRoot().width(),geom::max(0, relPtFromParent.x() - lastInnerDrawnAreaInRoot().width()/2)));
	}
}

bool ScrollCombo::onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance)
{
	geom::Point const& ptStart(this->scrollOffset_);
	geom::Point const ptEnd(this->scrollOffset_+this->innerSize());
	if(this->scrollMode_ & Vertical) {
		if (ptStart.y() < 0 || ptEnd.y() > this->childSize_.height()) {
			this->scrollOffset_.y(this->scrollOffset_.y() - (distance.y() / 2.0f));
		}else{
			this->scrollOffset_.y(this->scrollOffset_.y() - distance.y());
		}
	}
	if(this->scrollMode_ & Horizontal){
		if(ptStart.x() < 0 || ptEnd.x() > this->childSize_.width()) {
			this->scrollOffset_.x(this->scrollOffset_.x() - (distance.x()/2.0f));
		}else{
			this->scrollOffset_.x(this->scrollOffset_.x() - distance.x());
		}
	}
	this->lastMovedFrom_ = 0.0f;
	return true; //イベントを消費する
}



}}
