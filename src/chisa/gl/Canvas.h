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
#include <tarte/Exception.h>
#include <tarte/Logger.h>
#include <tarte/ClassUtil.h>
#include <tarte/Handler.h>
#include "../geom/Decl.h"
#include "Color.h"
#include "Sprite.h"
#include "Drawable.h"

namespace chisa {
namespace geom {
class Area;
}
namespace gl {

class Canvas {
	DISABLE_COPY_AND_ASSIGN(Canvas);
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	float width_;
	float height_;
public:
	Canvas(Logger& log);
	virtual ~Canvas();
public:
	void ortho(const float left, const float right, const float bottom, const float top, const float near_val, const float far_val);
	void resize2d(geom::Box const& box);
	void translate(geom::Point const& pt);
	void rotate(const float angle, geom::Point const& pt);
	void scale(geom::ScaleVector const& scale);
	void drawSprite(Handler<Sprite> sprite, geom::Point const& pt, const float depth = 0.0f, Color const& color=White);
	void drawSprite(Handler<Sprite> sprite, geom::Point const& pt, geom::Area const& spriteArea, const float depth = 0.0f, Color const& color=White);
	void drawLine(const float width, Color const& color, geom::Point const& start, geom::Point const& end, const float depth=0.0f);
	void drawRect(const float width, Color const& color, geom::Area const& area, const float depth=0.0f);
	void fillRect(Color const& color, geom::Area const& area, const float depth=0.0f);
	void drawTexture(unsigned int texId, geom::Point const& pt, geom::IntBox const& texSize, geom::Area const& spriteArea, const float depth, Color const& color);
	void drawTexture(unsigned int texId, geom::Point const& pt, geom::IntBox const& texSize, geom::IntBox const& spriteSize, const float depth, Color const& color);
private:
	void scissor(geom::Area const& area);
	void scissorReset();
	void setColor(Color const& color);
public:
	class ScissorScope {
	private:
		Canvas& canvas_;
	public:
		ScissorScope(Canvas& canvas, geom::Area const& area);
		virtual ~ScissorScope();
	};
	class AffineScope {
	private:
		Canvas& canvas_;
	public:
		AffineScope(Canvas& canvas);
		virtual ~AffineScope();
	};
};

}}
