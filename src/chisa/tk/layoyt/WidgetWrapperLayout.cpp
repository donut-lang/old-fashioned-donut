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

namespace chisa {
namespace tk {
namespace layout {

WidgetWrapperLayout::WidgetWrapperLayout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:Layout(log, world, root, parent)
,parent_(nullptr)
,widget_(nullptr)
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
	return(Box(100,100));
}
void WidgetWrapperLayout::reshapeImpl(const Area& area)
{
}
std::string WidgetWrapperLayout::toString()
{
	return "(WidgetWrapperLayout )";
}
void WidgetWrapperLayout::loadXML(LayoutFactory* const factory, XMLElement* const element)
{

}

}}}
