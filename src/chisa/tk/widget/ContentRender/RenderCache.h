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
#include "../../../Handler.h"
#include "../../../util/ClassUtil.h"
#include <deque>

namespace chisa {
namespace gl {
class Drawable;
class Sprite;
}

namespace tk {
namespace widget {

class RenderCache : public HandlerBody<RenderCache> {
	DISABLE_COPY_AND_ASSIGN(RenderCache)
private:
	const std::size_t maxSprites_;
	const std::size_t maxDrawable_;
	std::deque<Handler<gl::Drawable> > drawableCache_;
	std::deque<Handler<gl::Sprite> > spriteCache_;
public:
	RenderCache(const std::size_t maxSprites=100,const std::size_t maxDrawable=100);
	virtual ~RenderCache() noexcept = default;
public:
	void registerDrawable(Handler<gl::Drawable> d) noexcept;
	void registerSprite(Handler<gl::Sprite> s) noexcept;
public:
	bool unregisterDrawable(const Handler<gl::Drawable>& d) noexcept;
	bool unregisterSprite(const Handler<gl::Sprite>& s) noexcept;
public:
	void reset() noexcept;
public:
	void onFree();
};

}}}
