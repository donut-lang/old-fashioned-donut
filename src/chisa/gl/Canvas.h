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
#include <vector>
#include <deque>
#include <map>
#include "../logging/Exception.h"
#include "../logging/Logger.h"
#include "../util/ClassUtil.h"
#include "../geom/Decl.h"
#include "Color.h"
#include "Handler.h"
#include "Sprite.h"
#include "RawSprite.h"

namespace chisa {
namespace geom {
class Area;
}
namespace gl {

namespace internal {
struct BufferOrder {
	inline bool operator()(const Buffer* const a, const Buffer* const b) const noexcept
	{
		return a->width() == b->width() ? a->height() < b->height() : a->width() < b->width();
	}
	inline bool operator() (const Buffer* a, const std::pair<int,int>& b) const noexcept
	{
		return a->width() == b.first ? a->height() < b.second : a->width() < b.first;
	}
	inline bool operator() (const std::pair<int,int>& a, const Buffer* b) const noexcept
	{
		return a.first == b->width() ? a.second < b->height() : a.first < b->width();
	}
	inline bool operator() (const std::pair<int,int>& a, const std::pair<int,int>& b) const noexcept
	{
		return a.first == b.first ? a.second < b.second : a.first < b.first;
	}
};
struct SpriteOrder {
	inline bool operator()(const RawSprite* const a, const RawSprite* const b) const noexcept
	{
		return a->width() < b->width() && a->height() < b->height();
	}
	inline bool operator() (const RawSprite* a, const std::pair<int,int>& b) const noexcept
	{
		return a->width() < b.first && a->height() < b.second;
	}
	inline bool operator() (const std::pair<int,int>& a, const RawSprite* b) const noexcept
	{
		return a.first < b->width() && a.second < b->height();
	}
	inline bool operator() (const std::pair<int,int>& a, const std::pair<int,int>& b) const noexcept
	{
		return a.first < b.first && a.second < b.second;
	}
};
}

class Canvas {
	DISABLE_COPY_AND_ASSIGN(Canvas);
	DEFINE_MEMBER_REF(private, logging::Logger, log)
	float width_;
	float height_;
private:
	static constexpr size_t MaxCachedBufferCount = 200;
	static constexpr size_t MaxCachedSpriteCount = 200;
private:
	Handler<RawSprite> loadPNG(const std::string& filename);
public:
	Canvas(logging::Logger& log);
	virtual ~Canvas();
public:
	std::deque<RawSprite*> unusedSprite_;
	std::deque<Buffer*> unusedBuffer_;
	std::deque<std::pair<std::string, Handler<RawSprite> > > imageCache_;
public:
	void ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val);
	void resize2d(const geom::Box& box);
	void translate(const geom::Point& pt);
	void rotate(const float angle, const geom::Point& pt);
	void scale(const geom::ScaleVector& scale);
	void drawSprite(Handler<Sprite> sprite, const geom::Point& pt, const float depth=0.0f);
	void drawSprite(RawSprite* const sprite, const geom::Point& pt, const float depth=0.0f);
	void drawLine(const float width, const Color& color, const geom::Point& start, const geom::Point& end, const float depth=0.0f);
private:
	void scissor(const geom::Area& area);
	void scissorReset();
	void setColor(const Color& color);
public:
	class ScissorScope {
	private:
		Canvas& canvas_;
	public:
		ScissorScope(Canvas& canvas, const geom::Area& area);
		virtual ~ScissorScope();
	};
	class AffineScope {
	private:
		Canvas& canvas_;
	public:
		AffineScope(Canvas& canvas);
		virtual ~AffineScope();
	};
public:
	Handler<RawSprite> queryRawSprite(const int width, const int height);
	Handler<RawSprite> queryImage(const std::string& filename);
public: /* ハンドラやセッションから参照されるメソッド。普通触らない。 */
	Buffer* queryBuffer(const int width, const int height);
	void backSprite(RawSprite* spr);
	void backBuffer(Buffer* buffer);
};

}}
