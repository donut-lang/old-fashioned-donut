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

#ifndef Chisa_CANVAS_H__CPP_
#define Chisa_CANVAS_H__CPP_

#include <deque>
#include <map>
#include "../logging/Exception.h"
#include "../logging/Logger.h"
#include "../util/class_utils.h"
#include "../geom/Decl.h"
#include "Handler.h"
#include "Sprite.h"
#include "RawSprite.h"

namespace chisa {
namespace geom {
class Area;
}
namespace gl {

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
	void drawLine(const float width, const float r,const float g,const float b,const float a, const geom::Point& start, const geom::Point& end, const float depth=0.0f);
private:
	void scissor(const geom::Area& area);
	void scissorReset();
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
#endif /* INCLUDE_GUARD */
