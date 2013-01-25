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

#include "WidgetElement.h"
#include "../World.h"
#include "../../gl/Canvas.h"
#include "../Widget.h"

namespace chisa {
namespace tk {

static const std::string TAG("WidgetElement");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(WidgetElement, Element)
,borrowed_(nullptr)
,widget_(nullptr)
,fitMode_(Center)
,widgetScale_(1.0f, 1.0f)
{
	//this->backgroundColor(gl::Black);
}

WidgetElement::~WidgetElement() noexcept
{
	if(!this->widget()){
		return;
	}
}

void WidgetElement::idle(const float delta_ms)
{
	if(!widget()) {
		return;
	}
	this->widget()->idle(delta_ms);
}

void WidgetElement::renderImpl(gl::Canvas& canvas, geom::Point const& ptInScreen, geom::Area const& mask)
{
	if(!widget()){
		return;
	}
	gl::Canvas::AffineScope as(canvas);
	gl::Canvas::ScissorScope ss(canvas, geom::Area(ptInScreen+mask.point(), mask.box()));
	//スクリーン上の位置に移動
	canvas.translate(ptInScreen+this->widgetOffset());
	//areaからウィジットが実際にレンダリングされる位置を計算
	canvas.scale(this->widgetScale());
	//描画を投げる
	//TODO: offsetが0を下回るかもしれない。
	geom::Point offset(mask.point()-this->widgetOffset());
	geom::Box size(min(mask.box(), widgetSizeReal()-offset));
	this->widgetDrawnArea(geom::Area(offset / this->widgetScale(), size/this->widgetScale()));
	widget()->render(canvas, this->widgetDrawnArea());
}

geom::Box WidgetElement::measureImpl(geom::Box const& constraint)
{
	if(!widget()){
		return geom::Box(0,0);
	}
	geom::Box box(widget()->measure(constraint));
	if(geom::isUnspecified(box.width()) || geom::isUnspecified(box.height())){
		this->log().e(TAG, "Widget \"%s\" box size unspecified.", this->widgetId_.c_str());
	}
	switch (this->fitMode_)
	{
	case Expand: {
		const bool widthSpecified = geom::isSpecified(constraint.width());
		const bool heightSpecified = geom::isSpecified(constraint.height());
		float scale = 1;
		if(widthSpecified && heightSpecified){
			scale = this->calcScale(box, constraint);
		}else if(widthSpecified){
			scale = constraint.width() / box.width();
		}else if(heightSpecified){
			scale = constraint.height() / box.height();
		}
		return geom::Box(box * scale);
	}
	case Fit: {
		const bool widthSpecified = geom::isSpecified(constraint.width());
		const bool heightSpecified = geom::isSpecified(constraint.height());
		float scale = 1;
		if(widthSpecified && heightSpecified){
			scale = this->calcScale(box, constraint);
		}else if(widthSpecified){
			scale = constraint.width() / box.width();
		}else if(heightSpecified){
			scale = constraint.height() / box.height();
		}
		scale = std::min(scale, 1.0f);
		return geom::Box(box * scale);
	}
	case Center: {
		return box;
	}
	default: {
		this->log().e(TAG, "Unknwon fit mode: %d", this->fitMode_);
		return geom::Box(0,0);
	}
	}
}

float WidgetElement::calcScale(geom::Box const& widget, geom::Box const& constraint)
{
	//はみ出ないスケール
	return std::min(
			constraint.width() / widget.width(),
			constraint.height() / widget.height()
	);
}

void WidgetElement::notifyRelayoutFinished()
{
	Element::notifyRelayoutFinished();
	this->widget_->notifyRelayoutFinished();
}

void WidgetElement::layoutImpl(geom::Distance const& offsetFromParent, geom::Box const& size)
{
	if(!widget()){
		return;
	}
	this->widgetSize(widget()->measure(size));
	widget()->reshape(this->widgetSize()); //そのまま要求を飲む
	switch(this->fitMode_)
	{
	case Expand: {
		const float scale = this->calcScale(this->widgetSize(), size);

		this->widgetScale(geom::ScaleVector(scale, scale));
		this->widgetSizeReal(this->widgetSize() * scale);
		this->widgetOffset((size - this->widgetSizeReal())/2);
		break;
	}
	case Fit: {
		const float scale = this->calcScale(this->widgetSize(), size);

		this->widgetScale(geom::ScaleVector(scale, scale));
		this->widgetSizeReal(this->widgetSize() * scale);
		this->widgetOffset((size-this->widgetSizeReal())/2);
		break;
	}
	case Center: {
		this->widgetScale(geom::ScaleVector(1.0f, 1.0f));
		this->widgetSizeReal(this->widgetSize());
		this->widgetOffset((size - this->widgetSize())/2);
		break;
	}
	}
}

std::string WidgetElement::toString() const
{
	return ::tarte::format( "(WidgetElement \"%s\" %p)", this->widgetId_.c_str(), this);
}
void WidgetElement::loadXmlImpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
{
	if(element->Attribute("fit", "fit")){
		this->fitMode_ = Fit;
	}else if(element->Attribute("fit", "center")){
		this->fitMode_ = Center;
	}else if(element->Attribute("fit", "expand")){
		this->fitMode_ = Expand;
	}
	const char* widgetKlass = element->Attribute("widget-klass", nullptr);
	const char* widgetId = element->Attribute("widget-id", nullptr);
	if(!widgetKlass){
		this->log().e(TAG, "Oops. widget-klass not defined for id \"%s\".", widgetId);
		return;
	}
	if(Handler<World> world = this->world().lock()){
		Handler<WidgetElement> parent;
		if(widgetId && (parent = world->findWidgetById(widgetId)) ) {
			this->borrowed_ = parent;
			this->widget_ = parent->widget();
		}else{
			this->widget_ = world->createWidget(widgetKlass, element);
			this->widget_->updateWrapper( this->self() );
		}
		if( widgetId ) {
			world->replaceWidget(widgetId_ = widgetId, this);
		}
	}
}

geom::Point WidgetElement::calcPtInWidgetRel(geom::Point const& ptInScreen)
{
	const geom::Point delta = ptInScreen - this->lastInnerPositionInRoot() - this->widgetOffset();
	return geom::Point( delta / this->widgetScale() );
}

geom::Point WidgetElement::calcPtInWidgetAbs(geom::Point const& ptInScreen)
{
	return calcPtInWidgetRel(ptInScreen) + this->widgetOffset();
}

geom::Point WidgetElement::calcPtInElement(geom::Point const& ptInWidgetAbs)
{
	const geom::Point rel = ptInWidgetAbs + this->widgetOffset();
	return rel * this->widgetScale();
}

geom::Point WidgetElement::calcPtInRoot(geom::Point const& ptInWidgetAbs)
{
	return this->calcPtInElement(ptInWidgetAbs) + this->lastInnerPositionInRoot();
}

geom::Area WidgetElement::calcAreaInElement(geom::Area const& areaInWidget)
{
	return geom::Area( calcPtInElement(areaInWidget.point()), areaInWidget.box()*widgetScale() );
}
geom::Area WidgetElement::calcAreaInRoot(geom::Area const& areaInWidget)
{
	return geom::Area( calcPtInRoot(areaInWidget.point()), areaInWidget.box()*widgetScale() );
}

bool WidgetElement::onDownRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	return widget_->onDownRaw(timeMs, calcPtInWidgetRel(ptInScreen));
}

bool WidgetElement::onUpRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	return widget_->onUpRaw(timeMs, calcPtInWidgetRel(ptInScreen));
}

bool WidgetElement::onMoveRaw(float const& timeMs, geom::Point const& ptInScreen)
{
	return widget_->onMoveRaw(timeMs, calcPtInWidgetRel(ptInScreen));
}

bool WidgetElement::onSingleTapUp(float const& timeMs, geom::Point const& ptInScreen)
{
	return widget_->onSingleTapUp(timeMs, calcPtInWidgetRel(ptInScreen));
}

bool WidgetElement::onFling(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity)
{
	return widget_->onFling(timeMs, calcPtInWidgetRel(start), calcPtInWidgetRel(end), velocity / this->widgetScale());
}

bool WidgetElement::onScroll(float const& timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance)
{
	return widget_->onScroll(timeMs, calcPtInWidgetRel(start), calcPtInWidgetRel(end), distance / this->widgetScale());
}

bool WidgetElement::onZoom(float const& timeMs, geom::Point const& center, const float ratio)
{
	return widget_->onZoom(timeMs, calcPtInWidgetRel(center), ratio);
}

bool WidgetElement::onKeyDown(const float& timeMs, bool isRepeat, const SDL_Keysym& sym)
{
	return widget_->onKeyDown(timeMs, isRepeat, sym);
}

void chisa::tk::WidgetElement::onFocusGained(const float& timeMs, const geom::Point& lastPtInScreen)
{
	widget_->onFocusGained(timeMs, lastPtInScreen);
}

void chisa::tk::WidgetElement::onFocusLost(const float& timeMs)
{
	widget_->onFocusLost(timeMs);
}

void chisa::tk::WidgetElement::onShownImpl()
{
	widget_->onShown();
	//TODO: 借りる処理はここで行う
	Handler<World> const world = this->world().lock();
	if( !world && !widgetId_.empty() ) {
		world->replaceWidget(widgetId_, this);
	}
	this->widget()->updateWrapper( this->self() );
}

void chisa::tk::WidgetElement::onHiddenImpl()
{
	widget_->onHidden();
	if( !this->borrowed_.expired() ){ //借りた物をかえす
		Handler<World> const world = this->world().lock();
		if(world && world->replaceWidget(this->widgetId_, this->borrowed_) ) {
			this->widget()->updateWrapper( this->borrowed_ );
		}
	}
}

bool WidgetElement::onKeyUp(const float& timeMs, const SDL_Keysym& sym)
{
	return widget_->onKeyUp(timeMs, sym);
}

}}
