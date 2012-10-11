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
#include "WidgetWrapperLayout.h"
#include "../World.h"
#include "../../util/Param.h"
#include "../../gl/Canvas.h"

namespace chisa {
namespace tk {
namespace layout {

static const string TAG("WidgetWrapperLayout");

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(WidgetWrapperLayout)
,borrowed_(nullptr)
,widget_(nullptr)
,fitMode_(Center)
,widgetScaleX_(1.0f)
,widgetScaleY_(1.0f)
{
}

WidgetWrapperLayout::~WidgetWrapperLayout()
{
	if(!this->widget()){
		return;
	}
	if(this->borrowed_){
		if(shared_ptr<World> world = this->world().lock()){
			//ワールドの書き換えと、ウィジットへの現親レイアウトの通知
			if(world->replaceWidget(this->widgetId_, this->borrowed_)) {
				this->widget()->updateWrapper(dynamic_pointer_cast<WidgetWrapperLayout>(this->self().lock()));
			}
			// TODO　ウィジットにレイアウト通知入れたほうがいい？？
		}
	}else{
		if(shared_ptr<World> world = this->world().lock()){
			//ワールドからの削除と、ウィジットの開放
			if(world->deleteWidget(this->widgetId_, this)){
				delete widget();
				this->widget(nullptr);
			}
		}
	}
}

weak_ptr<Layout> WidgetWrapperLayout::getChildAt(const size_t index) const
{
	return weak_ptr<Layout>();
}
size_t WidgetWrapperLayout::getChildCount() const
{
	return 0;
}
void WidgetWrapperLayout::idle(const float delta_ms)
{
	if(!widget()) {
		return;
	}
	this->widget()->idle(delta_ms);
}
void WidgetWrapperLayout::renderImpl(gl::Canvas& canvas, const geom::Area& screenArea, const geom::Area& area)
{
	if(!widget()){
		return;
	}
	canvas.pushMatrix();
	{
		gl::Canvas::Scissor ss(canvas, screenArea);
		//スクリーン上の位置に移動
		canvas.translate(screenArea.x(), screenArea.y(), 0.0f);
		//areaからウィジットが実際にレンダリングされる位置を計算
		canvas.scale(this->widgetScaleX(), this->widgetScaleY(), 1.0);
		//描画を投げる
		geom::Vector offset(area.x()-this->widgetOffset().x(), area.y()-this->widgetOffset().y());
		geom::Box size(std::min(area.width(), widgetSizeReal().width()-area.x()), std::min(area.height(), widgetSizeReal().height()-area.y()));
		this->widgetDrawnArea(geom::Area(offset.x() / this->widgetScaleX(), offset.y() / this->widgetScaleY(), size.width()/this->widgetScaleX(), size.height()/this->widgetScaleY()));
		widget()->render(canvas, this->widgetDrawnArea());
	}
	canvas.popMatrix();
}

geom::Box chisa::tk::layout::WidgetWrapperLayout::onMeasure(const geom::Box& constraint)
{
	if(!widget()){
		return geom::Box(0,0);
	}
	geom::Box box(widget()->measure(constraint));
	if(geom::isUnspecified(box.width()) || geom::isUnspecified(box.height())){
		this->log().e(TAG, "Widget \"this->widgetId_.c_str()\" box size unspecified.");
	}
	if(this->fitMode_ == Fit){
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
	}else if(this->fitMode_ == Center){
		return box;
	}else{
		this->log().e(TAG, "Unknwon fit mode: %d", this->fitMode_);
		return geom::Box(0,0);
	}
}

float WidgetWrapperLayout::calcScale(const geom::Box& widget, const geom::Box& constraint)
{
	if(constraint > this->widgetSize()){ //完全に小さい
		// 拡大。
		return std::min(
				constraint.width() / widget.width(),
				constraint.height() / widget.height()
		);
	}else{
		//そうでない場合は長い辺に合わせて縮小。
		return std::min(
				constraint.width() / widget.width(),
				constraint.height() / widget.height()
		);
	}
}

void WidgetWrapperLayout::onLayout(const geom::Box& size)
{
	if(!widget()){
		return;
	}
	this->widgetSize(widget()->measure(size));
	widget()->reshape(this->widgetSize()); //そのまま要求を飲む
	switch(this->fitMode_)
	{
	case Fit: {
		const float scale = this->calcScale(this->widgetSize(), size);
		this->widgetScaleX(scale);
		this->widgetScaleY(scale);
		this->widgetSizeReal(geom::Box(this->widgetSize().width() * scale, this->widgetSize().height() * scale));
		this->widgetOffset(geom::Vector((size.width() - this->widgetSizeReal().width())/2, (size.height()-this->widgetSizeReal().height())/2));
		break;
	}
	case Center: {
		this->widgetScaleX(1.0f);
		this->widgetScaleY(1.0f);
		this->widgetSizeReal(this->widgetSize());
		this->widgetOffset(geom::Vector((size.width() - this->widgetSize().width())/2, (size.height()-this->widgetSize().height())/2));
		break;
	}
	}
}

std::string WidgetWrapperLayout::toString()
{
	return util::format( "(WidgetWrapperLayout %s)", this->widgetId_.c_str());
}
void WidgetWrapperLayout::loadXMLimpl(LayoutFactory* const factory, XMLElement* const element)
{
	if(element->Attribute("fit", "fit")){
		this->fitMode_ = Fit;
	}else if(element->Attribute("fit", "center")){
		this->fitMode_ = Center;
	}
	const char* widgetKlass = element->Attribute("widget-klass", nullptr);
	const char* widgetId = element->Attribute("widget-id", nullptr);
	if(!widgetKlass){
		this->log().e(TAG, "Oops. widget-klass not defined for id \"%s\".", widgetId);
		return;
	}
	if(shared_ptr<World> world = this->world().lock()){
		if(widgetId && (this->borrowed_ = world->getWidgetById(widgetId))){
			world->replaceWidget(widgetId, this);
			this->widget(this->borrowed_->widget());
			this->widget()->updateWrapper(dynamic_pointer_cast<WidgetWrapperLayout>(this->self().lock()));
		}else{
			this->widget(world->createWidget(widgetKlass, element));
			if(!this->widget()){
				this->log().e(TAG, "Oops. widget \"%s\" not registered.", widgetKlass);
			}
		}
	}
}

weak_ptr<Layout> WidgetWrapperLayout::getLayoutByIdImpl(const std::string& id)
{
	return weak_ptr<Layout>();
}

geom::Vector WidgetWrapperLayout::calcPtInWidget(const geom::Vector& ptInScreen)
{
	geom::Vector delta = ptInScreen - this->screenArea().point();
	geom::Vector scaledDelta(delta.x() / this->widgetScaleX(), delta.y() / this->widgetScaleY());
	return geom::Vector(this->widgetDrawnArea().point() + scaledDelta);
}

bool WidgetWrapperLayout::onDownRaw(const float timeMs, const geom::Vector& ptInScreen)
{
	return widget_->onDownRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetWrapperLayout::onUpRaw(const float timeMs, const geom::Vector& ptInScreen)
{
	return widget_->onUpRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetWrapperLayout::onMoveRaw(const float timeMs, const geom::Vector& ptInScreen)
{
	return widget_->onMoveRaw(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetWrapperLayout::onSingleTapUp(const float timeMs, const geom::Vector& ptInScreen)
{
	return widget_->onSingleTapUp(timeMs, calcPtInWidget(ptInScreen));
}

bool WidgetWrapperLayout::onFling(const float timeMs, const geom::Vector& start, const geom::Vector& end, const geom::Velocity& velocity)
{
	const geom::Velocity scaledVelocity(velocity.x() / this->widgetScaleX(), velocity.y() / this->widgetScaleY());
	return widget_->onFling(timeMs, calcPtInWidget(start), calcPtInWidget(end), scaledVelocity);
}

bool WidgetWrapperLayout::onScroll(const float timeMs, const geom::Vector& start, const geom::Vector& end, const geom::Vector& distance)
{
	const geom::Vector scaledDistance(distance.x() / this->widgetScaleX(), distance.y() / this->widgetScaleY());
	return widget_->onScroll(timeMs, calcPtInWidget(start), calcPtInWidget(end), scaledDistance);
}

bool WidgetWrapperLayout::onZoom(const float timeMs, const geom::Vector& center, const float ratio)
{
	return widget_->onZoom(timeMs, calcPtInWidget(center), ratio);
}

}}}
