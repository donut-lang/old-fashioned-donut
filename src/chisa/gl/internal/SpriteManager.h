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
#include <deque>
#include <tarte/ClassUtil.h>
#include "../ImageFormat.h"

namespace chisa {
using namespace tarte;

namespace gl {
class Sprite;

namespace internal {
class Buffer;

class SpriteManager: public HandlerBody<SpriteManager> {
	DEFINE_MEMBER_REF(private, Logger, log)
	Canvas& canvas_;
private:
	static constexpr size_t MaxCachedSpriteCount = 200;
	static constexpr size_t MaxCachedBufferCount = 200;
	std::deque<Sprite*> unusedSprite_;
	std::deque<internal::Buffer*> unusedBuffer_;
public:
	SpriteManager(Logger& log, Canvas& canvas);
	virtual ~SpriteManager() noexcept;
	inline bool onFree() noexcept { return false; };
	Handler<Sprite> queryRawSprite(ImageFormat const foramt, const int width, const int height);
public:
	void backSprite(Sprite* spr);
	internal::Buffer* queryBuffer(std::size_t size);
	void backBuffer(internal::Buffer* buffer);
};

}}}
