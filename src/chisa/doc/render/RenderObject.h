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

#pragma once
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include "../../geom/Area.h"

namespace chisa {
namespace gl {
class Canvas;
}

namespace doc {
class RenderTree;
class Node;

class RenderObject : public HandlerBody<RenderObject> {
private:
	HandlerW<RenderTree> parentTree_;
	HandlerW<Node> parentNode_;
	DEFINE_MEMBER(public, public, geom::Area, area);
	DEFINE_MEMBER(public, public, float, relDepth);
public:
	RenderObject(HandlerW<RenderTree> parentTree, HandlerW<Node> parentNode, const float relDepth);
	virtual ~RenderObject() noexcept = default;
public:
	void onFree() noexcept { delete this; };
	virtual void render(gl::Canvas& canvas, const geom::Point& offset, const float depth) = 0;
	virtual void onHidden() = 0;
};

}}
