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
#include <map>
#include <cairo/cairo.h>
#include "../geom/Area.h"
#include "Color.h"
#include "../Handler.h"
#include "../logging/Logger.h"
#include "Font.h"

namespace chisa {
namespace gl {
class Canvas;
class Sprite;
class DrawableManager;

class Drawable : public HandlerBody<Drawable> {
	HandlerW<DrawableManager> manager_;
protected:
	Drawable(HandlerW<DrawableManager> manager):manager_(manager){};
	HandlerW<DrawableManager>& manager() { return this->manager_; };
public:
	virtual ~Drawable() noexcept = default;
	inline void onFree() { delete this; }
	float width();
	float height();
	virtual geom::Box size() = 0;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) = 0;
	virtual std::string toString() const = 0;
};

class NullDrawable : public Drawable {
private:
	geom::Box size_;
protected:
	NullDrawable( HandlerW<DrawableManager> manager, const geom::Box& size ):Drawable(manager), size_(size){};
public:
	virtual ~NullDrawable() noexcept = default;
	virtual geom::Box size() override { return this->size_; };
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override {};
	virtual std::string toString() const override;
	static Handler<Drawable> create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl );
};

class ColorDrawable : public Drawable {
private:
	geom::Box size_;
	Color color_;
	ColorDrawable( HandlerW<DrawableManager> manager, const geom::Box size, const gl::Color& c);
public:
	static Handler<Drawable> create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl );
	virtual ~ColorDrawable() noexcept = default;
public:
	Color color() const noexcept;
	virtual geom::Box size() override { return this->size_; };
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const override;
};

class ImageDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<gl::Sprite> sprite_;
	std::string filename_;
protected:
	ImageDrawable( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& filename);
	virtual ~ImageDrawable() noexcept = default;
public:
	static Handler<Drawable> create(  HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl );
	Handler<gl::Sprite> sprite();
	virtual geom::Box size() override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const override;
};

class RepeatDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<Drawable> child_;
	RepeatDrawable(HandlerW<DrawableManager> manager, const geom::Box& size, Handler<Drawable> child);
public:
	static Handler<Drawable> create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl );
	virtual ~RepeatDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const override;
};

class StretchDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<Drawable> child_;
	StretchDrawable(HandlerW<DrawableManager> manager, const geom::Box size, Handler<Drawable> child);
public:
	static Handler<Drawable> create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl );
	virtual ~StretchDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const override;
};

class TextDrawable : public Drawable {
public:
	enum Style {
		Regular = 0,
		Bold = 1,
		Italic = 2
	};
	enum Decoration {
		None = 0,
		Underline = 1,
		Strike = 2
	};
	static const float DefaultFontSize;
private:
	Handler<gl::Sprite> sprite_;
	geom::Area renderInfo_;
	std::string str_;
	bool vertical_;
	float size_;
	Handler<Font> font_;
	Style style_;
	Decoration deco_;
	gl::Color color_;
	gl::Color backColor_;
private:
	TextDrawable( HandlerW<DrawableManager> manager, const std::string& str, bool vertical, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, const gl::Color& color, const gl::Color& backColor);
	void revalidate();
public:
	virtual ~TextDrawable() noexcept = default;
	static void setupCairo(cairo_t* cairo, cairo_font_face_t* face, cairo_font_options_t* opt, float size, Style style);
	static Handler<TextDrawable> create(HandlerW<DrawableManager> manager, const std::string& str, bool vertical, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, const gl::Color& color, const gl::Color& backColor);
	virtual geom::Box size() override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const override;
	Handler<gl::Sprite> sprite();
};

}}
