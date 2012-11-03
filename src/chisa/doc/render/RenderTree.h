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
#include "../../util/ClassUtil.h"
#include "../../Handler.h"
#include "../../logging/Logger.h"
#include <deque>
#include <vector>
#include "../../gl/Drawable.h"
#include "RenderObject.h"

namespace chisa {
namespace gl {
class Drawable;
class Sprite;
class Canvas;
}

namespace doc {
class RenderObject;

class RenderTree : public HandlerBody<RenderTree> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	const std::size_t maxDrawable_;
	std::deque<Handler<gl::Drawable> > drawableCache_;
	std::vector<Handler<RenderObject> > objects_;
public:
	RenderTree(logging::Logger& log, const std::size_t maxDrawable=100);
	virtual ~RenderTree() noexcept = default;
public:
	void render(gl::Canvas& canvas, const geom::Area& area, float depth);
public:
	void registerDrawable(Handler<gl::Drawable> d) noexcept;
	void reset() noexcept;
	void onFree();
public:
	Handler<RenderObject> newDrawable(const geom::Area& area, const std::string& drawableRepl);
};

}}
