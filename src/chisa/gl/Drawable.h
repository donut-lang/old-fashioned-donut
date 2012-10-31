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
#include "../geom/Vector.h"
#include "Color.h"
#include "../Handler.h"

namespace chisa {
namespace gl {
class Canvas;
class RawSprite;

class Drawable : public HandlerBody<Drawable> {
public:
	Drawable() = default;
	virtual ~Drawable() noexcept = default;
public:
	inline void onFree() { delete this; }
	float width() const noexcept;
	float height() const noexcept;
	virtual geom::Box size() const noexcept = 0;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) = 0;
};

class ColorDrawable : public Drawable {
private:
	Color color_;
	geom::Box size_;
public:
	ColorDrawable(const Color& c, const geom::Box size);
	virtual ~ColorDrawable() noexcept = default;
public:
	Color color() const noexcept;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
};

class SpriteDrawable : public Drawable {
private:
	Handler<gl::RawSprite> sprite_;
public:
	SpriteDrawable(Handler<gl::RawSprite> spr);
	virtual ~SpriteDrawable() noexcept = default;
public:
	Handler<gl::RawSprite> sprite() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
};

class RepeatDrawable : public Drawable {
private:
	Handler<Drawable> child_;
	geom::Box size_;
public:
	RepeatDrawable(Handler<Drawable> child, const geom::Box& size);
	virtual ~RepeatDrawable() noexcept = default;
public:
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
};

class StretchDrawable : public Drawable {
private:
	Handler<Drawable> child_;
	geom::Box size_;
public:
	StretchDrawable(Handler<Drawable> child, const geom::Box& size);
	virtual ~StretchDrawable() noexcept = default;
public:
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
};

}}
