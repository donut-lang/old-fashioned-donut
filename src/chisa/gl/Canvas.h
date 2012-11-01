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
#include "../Handler.h"
#include "Sprite.h"
#include "Drawable.h"

namespace chisa {
namespace geom {
class Area;
}
namespace gl {


class Canvas {
	DISABLE_COPY_AND_ASSIGN(Canvas);
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	float width_;
	float height_;
private:
	Handler<Sprite> loadPNG(const std::string& filename);
public:
	Canvas(logging::Logger& log);
	virtual ~Canvas();
public:
	SpriteManager spriteManager_;
	DrawableFactory drawableFactory_;
	std::deque<std::pair<std::string, Handler<Sprite> > > imageCache_;
public:
	void ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val);
	void resize2d(const geom::Box& box);
	void translate(const geom::Point& pt);
	void rotate(const float angle, const geom::Point& pt);
	void scale(const geom::ScaleVector& scale);
	void drawSprite(Handler<Sprite> sprite, const geom::Point& pt, const float depth = 0.0f);
	void drawSprite(Handler<Sprite> sprite, const geom::Point& pt, const geom::Area& spriteArea, const float depth = 0.0f);
	void drawLine(const float width, const Color& color, const geom::Point& start, const geom::Point& end, const float depth=0.0f);
	void drawRect(const float width, const Color& color, const geom::Area& area, const float depth=0.0f);
	void fillRect(const Color& color, const geom::Area& area, const float depth=0.0f);
	void drawTexture(unsigned int texId, const geom::Point& pt, const geom::IntBox& texSize, const geom::Area& spriteArea, const float depth);
	void drawTexture(unsigned int texId, const geom::Point& pt, const geom::IntBox& texSize, const geom::IntBox& spriteSize, const float depth);
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
	Handler<Sprite> queryRawSprite(const int width, const int height);
	Handler<Sprite> queryImage(const std::string& filename);
public:
	Handler<Drawable> queryDrawable(const std::string& repl, const geom::Box& box=geom::Box());
};

}}
