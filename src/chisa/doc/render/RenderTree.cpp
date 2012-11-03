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

#include "RenderTree.h"

namespace chisa {
namespace doc {

const static std::string TAG("RenderTree");

RenderTree::RenderTree(logging::Logger& log, const std::size_t maxDrawable)
:log_(log)
,maxDrawable_(maxDrawable)
{
}

void RenderTree::render(gl::Canvas& canvas, const geom::Area& area, float depth)
{
	for(Handler<RenderObject>& obj : this->objects_){
		geom::Area const intersect(obj->area().intersect(area));
		if (!intersect.empty()) {
			obj->render(canvas, area.point(), depth+obj->relDepth());
		}else{
			obj->onHidden();
		}
	}
}


void RenderTree::registerDrawable(Handler<gl::Drawable> d) noexcept
{
	this->drawableCache_.push_back(d);
	while(this->drawableCache_.size() > this->maxDrawable_){
		if(this->log().t()){
			Handler<gl::Drawable> del = this->drawableCache_.front();
			this->log().t(TAG, "Drawable cache deleted: %s", del->toString().c_str());
		}
		this->drawableCache_.pop_front();
	}
}

void RenderTree::reset() noexcept
{
	(decltype(this->drawableCache_)()).swap(this->drawableCache_);
}

void RenderTree::onFree()
{
	this->reset();
	delete this;
}

Handler<RenderObject> RenderTree::newDrawable(const geom::Area& area, const std::string& drawableRepl)
{

}


}}
