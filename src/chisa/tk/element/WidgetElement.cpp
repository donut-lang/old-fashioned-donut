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

#include "GL/gl.h"
#include "WidgetElement.h"
#include "../World.h"
#include "../../util/Param.h"
#include "../../gl/Canvas.h"

namespace chisa {
namespace tk {
namespace element {

static const std::string TAG("WidgetElement");

CHISA_ELEMENT_SUBKLASS_CONSTRUCTOR_DEF_DERIVED(WidgetElement, LeafElement)
,borrowed_(nullptr)
,widget_(nullptr)
,fitMode_(Center)
,widgetScale_(1.0f, 1.0f)
{
}

WidgetElement::~WidgetElement() noexcept
{
	if(!this->widget()){
		return;
	}
	if(this->borrowed_){
		if(std::shared_ptr<World> world = this->world().lock()){
			//ワールドの書き換えと、ウィジットへの現親レイアウトの通知
			if(world->replaceWidget(this->widgetId_, this->borrowed_)) {
				this->widget()->updateWrapper(std::dynamic_pointer_cast<WidgetElement>(this->self().lock()));
			}
			// TODO　ウィジットにレイアウト通知入れたほうがいい？？
		}
	}else{
		//このラッパの所属するワールドが所有権を持つので、何もせず黙って削除。
		// INFO: ここで上のワールドはすでに開放済みなので、さわれません。
		delete widget();
		this->widget(nullptr);
	}
}

std::weak_ptr<Element> WidgetElement::getChildAt(const std::size_t index) const
{
	return std::weak_ptr<Element>();
}
std::size_t WidgetElement::getChildCount() const
{
	return 0;
}
void WidgetElement::idle(const float delta_ms)
{
	if(!widget()) {
		return;
	}
	this->widget()->idle(delta_ms);
}
void WidgetElement::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	if(!widget()){
		return;
	}
	gl::Canvas::AffineScope as(canvas);
	gl::Canvas::ScissorScope ss(canvas, screenArea);
	//スクリーン上の位置に移動
	canvas.translate(screenArea.point());
	//areaからウィジットが実際にレンダリングされる位置を計算
	canvas.scale(this->widgetScale());
	//描画を投げる
	//TODO: offsetが0を下回るかもしれない。
	geom::Point offset(area.point()-this->widgetOffset());
	using namespace chisa::geom;
	geom::Box size(min(area.box(), widgetSizeReal()-offset));
	this->widgetDrawnArea(geom::Area(offset / this->widgetScale(), size/this->widgetScale()));
	widget()->render(canvas, this->widgetDrawnArea());
}

geom::Box WidgetElement::onMeasure(const geom::Box& constraint)
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
		return geom::Box(box.width() * scale, box.height() * scale);
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
		return geom::Box(box.width() * scale, box.height() * scale);
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

float WidgetElement::calcScale(const geom::Box& widget, const geom::Box& constraint)
{
	//はみ出ないスケール
	return std::min(
			constraint.width() / widget.width(),
			constraint.height() / widget.height()
	);
}

void WidgetElement::onLayout(const geom::Box& size)
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
		this->widgetSizeReal(geom::Box(this->widgetSize().width() * scale, this->widgetSize().height() * scale));
		this->widgetOffset(geom::Point((size.width() - this->widgetSizeReal().width())/2, (size.height()-this->widgetSizeReal().height())/2));
		break;
	}
	case Fit: {
		const float scale = this->calcScale(this->widgetSize(), size);

		this->widgetScale(geom::ScaleVector(scale, scale));
		this->widgetSizeReal(geom::Box(this->widgetSize().width() * scale, this->widgetSize().height() * scale));
		this->widgetOffset(geom::Point((size.width() - this->widgetSizeReal().width())/2, (size.height()-this->widgetSizeReal().height())/2));
		break;
	}
	case Center: {
		this->widgetScale(geom::ScaleVector(1.0f, 1.0f));
		this->widgetSizeReal(this->widgetSize());
		this->widgetOffset(geom::Point((size.width() - this->widgetSize().width())/2, (size.height()-this->widgetSize().height())/2));
		break;
	}
	}
}

std::string WidgetElement::toString() const
{
	return util::format( "(WidgetElement \"%s\")", this->widgetId_.c_str());
}
void WidgetElement::loadXMLimpl(ElementFactory* const factory, tinyxml2::XMLElement* const element)
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
	if(std::shared_ptr<World> world = this->world().lock()){
		if(widgetId && (this->borrowed_ = world->getWidgetById(widgetId))){
			world->replaceWidget(widgetId, this);
			this->widget(this->borrowed_->widget());
			this->widget()->updateWrapper(std::dynamic_pointer_cast<WidgetElement>(this->self().lock()));
		}else{
			this->widget(world->createWidget(widgetKlass, element));
			if(!this->widget()){
				this->log().e(TAG, "Oops. widget \"%s\" not registered.", widgetKlass);
			}
		}
	}
}

std::weak_ptr<Element> WidgetElement::getElementByIdImpl(const std::string& id)
{
	return std::weak_ptr<Element>();
}

geom::Point WidgetElement::calcPtInWidget(const geom::Point& ptInScreen)
{
	const geom::Point delta = ptInScreen - this->screenArea().point();
	return geom::Point(this->widgetDrawnArea().point() + delta / this->widgetScale());
}

bool WidgetElement::onDownRaw(const float timeMs, const geom::Point& ptInScreen)
{
	return widget_->onDownRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetElement::onUpRaw(const float timeMs, const geom::Point& ptInScreen)
{
	return widget_->onUpRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetElement::onMoveRaw(const float timeMs, const geom::Point& ptInScreen)
{
	return widget_->onMoveRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetElement::onSingleTapUp(const float timeMs, const geom::Point& ptInScreen)
{
	return widget_->onSingleTapUp(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetElement::onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity)
{
	return widget_->onFling(timeMs, calcPtInWidget(start), calcPtInWidget(end), velocity / this->widgetScale());
}

bool WidgetElement::onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance)
{
	return widget_->onScroll(timeMs, calcPtInWidget(start), calcPtInWidget(end), distance / this->widgetScale());
}

bool WidgetElement::onZoom(const float timeMs, const geom::Point& center, const float ratio)
{
	return widget_->onZoom(timeMs, calcPtInWidget(center), ratio);
}

}}}
