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

namespace chisa {
namespace tk {
namespace layout {

static const string TAG("WidgetWrapperLayout");

CHISA_LAYOUT_SUBKLASS_CONSTRUCTOR_DEF(WidgetWrapperLayout)
,parent_(nullptr)
,widget_(nullptr)
,fitMode_(Center)
{
}

WidgetWrapperLayout::~WidgetWrapperLayout()
{
	if(!this->widget_){
		return;
	}
	if(this->parent_){
		if(shared_ptr<World> world = this->world().lock()){
			//ワールドの書き換えと、ウィジットへの現親レイアウトの通知
			if(world->replaceWidget(this->widgetId_, this->parent_)) {
				this->widget_->updateWrapper(this->self());
			}
			this->widget_->reshape(this->parent_->area().box());
		}
	}else{
		if(shared_ptr<World> world = this->world().lock()){
			//ワールドからの削除と、ウィジットの開放
			if(world->deleteWidget(this->widgetId_, this)){
				delete widget_;
				this->widget_ = nullptr;
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
void WidgetWrapperLayout::render(const Area& area)
{
	//glScissor(area.x(), area.y(), area.width(), area.height());
	//glPushMatrix();
	{
	}
	//glPopMatrix();
}
void WidgetWrapperLayout::idle(const float delta_ms)
{

}
Box WidgetWrapperLayout::measure(const Box& constraint)
{
	Box box(widget_->measure(constraint));
	if(geom::isUnspecified(box.width()) || geom::isUnspecified(box.height())){
		log().e(TAG, "Widget \"this->widgetId_.c_str()\" box size unspecified.");
	}
	return box;
}
void WidgetWrapperLayout::reshapeImpl(const Area& area)
{
}
std::string WidgetWrapperLayout::toString()
{
	return util::format( "(WidgetWrapperLayout %s)", this->widgetId_.c_str());
}
void WidgetWrapperLayout::loadXML(LayoutFactory* const factory, XMLElement* const element)
{
	if(element->Attribute("fit", "fit")){
		this->fitMode_ = Fit;
	}else if(element->Attribute("fit", "center")){
		this->fitMode_ = Center;
	}
	const char* widgetKlass = element->Attribute("widget-klass", nullptr);
	if(widgetKlass){
		this->log().e(TAG, "Oops. widget-klass not defined.");
		return;
	}
	const char* widgetId = element->Attribute("widget-id", nullptr);
	if(shared_ptr<World> world = this->world().lock()){
		if(widgetId && (this->parent_ = world->getWidgetById(widgetId))){
			world->replaceWidget(widgetId, this);
			this->widget_ = this->parent_->widget_;
			this->widget_->updateWrapper(this->self());
		}else{
			this->widget_ = world->createWidget(widgetKlass, element);
			if(!this->widget_){
				this->log().e(TAG, "Oops. widget \"%s\" not registered.", widgetKlass);
			}
		}
	}
}

}}}
