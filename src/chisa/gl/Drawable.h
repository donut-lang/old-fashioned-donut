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
#include <map>
#include "../logging/Logger.h"

namespace chisa {
namespace gl {
class Canvas;
class Sprite;
class DrawableInterpreter;

class Drawable : public HandlerBody<Drawable> {
protected:
	Drawable() = default;
public:
	virtual ~Drawable() noexcept = default;
	inline void onFree() { delete this; }
	float width() const noexcept;
	float height() const noexcept;
	virtual geom::Box size() const noexcept = 0;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) = 0;
	virtual std::string toString() const noexcept = 0;
};

class NullDrawable : public Drawable {
private:
	geom::Box size_;
protected:
	NullDrawable( const geom::Box& size ):size_(size){};
public:
	virtual ~NullDrawable() noexcept = default;
	virtual geom::Box size() const noexcept override { return this->size_; };
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override {};
	virtual std::string toString() const noexcept override;
	static Handler<Drawable> create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl );
};

class ColorDrawable : public Drawable {
private:
	geom::Box size_;
	Color color_;
	ColorDrawable(const geom::Box size, const gl::Color& c);
public:
	static Handler<Drawable> create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl );
	virtual ~ColorDrawable() noexcept = default;
public:
	Color color() const noexcept;
	virtual geom::Box size() const noexcept override { return this->size_; };
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class ImageDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<gl::Sprite> sprite_;
	std::string filename_;
protected:
	ImageDrawable(const geom::Box& size, const std::string& filename);
	virtual ~ImageDrawable() noexcept = default;
public:
	static Handler<Drawable> create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl );
	Handler<gl::Sprite> sprite() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class RepeatDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<Drawable> child_;
	RepeatDrawable(const geom::Box& size, Handler<Drawable> child);
public:
	static Handler<Drawable> create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl );
	virtual ~RepeatDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class StretchDrawable : public Drawable {
private:
	geom::Box size_;
	Handler<Drawable> child_;
	StretchDrawable(const geom::Box size, Handler<Drawable> child);
public:
	static Handler<Drawable> create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl );
	virtual ~StretchDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
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

};

//-------------------------------------------------------------------------------------------------

class DrawableInterpreter
{
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	typedef std::function<Handler<Drawable>(DrawableInterpreter&, const geom::Box&, const std::string&)> constructor;
	std::map<std::string, constructor> factories_;
public:
	DrawableInterpreter(logging::Logger& log);
	~DrawableInterpreter() = default;
public:
	Handler<Drawable> queryDrawable( const geom::Box& size, const std::string& repl );
};

}}
