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

#include "Layout.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {

Layout::Layout(logging::Logger& log, weak_ptr<World> world, weak_ptr<Layout> root, weak_ptr<Layout> parent)
:log_(log)
,world_(world)
,root_(root)
,parent_(parent)
{

}

void Layout::init(weak_ptr<Layout> _self)
{
	this->self_.swap(_self);
}
Layout::~Layout()
{

}

void Layout::idle(const float delta_ms)
{
	const size_t max = this->getChildCount();
	for(size_t i=0;i<max;++i){
		if( shared_ptr<Layout> child = this->getChildAt(i).lock()){
			child->idle(delta_ms);
		}
	}
}

void Layout::loadXML(layout::LayoutFactory* const factory, tinyxml2::XMLElement* const element)
{

	if(const char* id = element->Attribute("id", nullptr)){
		this->id(id);
	}
	this->loadXMLimpl(factory, element);
}

weak_ptr<Layout> Layout::getLayoutById(const std::string& id)
{
	return id == this->id() ? this->self() : this->getLayoutByIdImpl(id);
}

string Layout::toString()
{
	return util::format("(Layout %p)", this);
}

Box Layout::measure(const Box& constraint)
{
	Box box(this->onMeasure(constraint));
	return Box(
			box.width() > constraint.width() ? constraint.width() : box.width(),
			box.height() > constraint.height() ? constraint.height() : box.height()
		);
}
void Layout::render(gl::Canvas& canvas, const Area& screenArea, const Area& area)
{
	this->screenArea(screenArea);
	this->drawnArea(area);
	if( //描画範囲にないので書く必要性がありません。
		area.height()+area.y() <= 0 ||
		area.width()+area.x() <= 0 ||
		area.x() >= this->size().width() ||
		area.y() >= this->size().height() ){
		return;
	}
	this->renderImpl(canvas, screenArea, area);
}

void Layout::layout(const Box& size)
{
	this->onLayout(size);
	this->size(size);
}


}}
