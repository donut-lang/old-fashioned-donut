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

#include "RenderCommand.h"
#include "RenderTree.h"

namespace chisa {
namespace tk {
namespace widget {


RenderTree::RenderTree()
{
}

RenderTree::~RenderTree() noexcept(true)
{
	this->reset();
}

void RenderTree::reset() noexcept(true)
{
	for(RenderCommand* cmd : this->objects_){
		delete cmd;
	}
	decltype(this->objects_)().swap(this->objects_);
}

void RenderTree::append(RenderCommand* cmd)
{
	this->objects_.push_back(cmd);
}
void RenderTree::render(gl::Canvas& cv, const geom::Area& area)
{
	const float limit = area.height()/2;
	for(RenderCommand* cmd : this->objects_){
		geom::Area const intersect(cmd->area().intersect(area));
		if(!intersect.empty()){
			cmd->execute(cv, area.point());
		}else{
			const geom::Vector distV = geom::max(cmd->area().point() - area.point() - area.box(), area.point() - cmd->area().point() - cmd->area().box());
			const float dist = std::max(distV.x(), distV.y());
			if(dist > limit){
				cmd->onHidden();
			}
		}
	}
}

}}}
