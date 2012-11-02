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
class DrawableFactory;

class Drawable : public HandlerBody<Drawable> {
private:
	DEFINE_MEMBER(protected, protected, geom::Box, specSize);
protected:
	Drawable( const geom::Box& size ):specSize_(size){};
public:
	virtual ~Drawable() noexcept = default;
	inline void onFree() { delete this; }
	float width() const noexcept;
	float height() const noexcept;
	virtual geom::Box size() const noexcept { return this->specSize_; }
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) = 0;
	virtual std::string toString() const noexcept = 0;
};

class NullDrawable : public Drawable {
protected:
	NullDrawable( const geom::Box& size ):Drawable(size){};
public:
	virtual ~NullDrawable() noexcept = default;
	inline void onFree() { delete this; }
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) {};
	virtual std::string toString() const noexcept override;
	static Handler<Drawable> create( DrawableFactory& factory, const geom::Box& size, const std::string& repl );
};

class ColorDrawable : public Drawable {
private:
	Color color_;
	ColorDrawable(const geom::Box size, const gl::Color& c);
public:
	static Handler<Drawable> create( DrawableFactory& factory, const geom::Box& size, const std::string& repl );
	virtual ~ColorDrawable() noexcept = default;
public:
	Color color() const noexcept;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class ImageDrawable : public Drawable {
private:
	Handler<gl::Sprite> sprite_;
	std::string filename_;
	geom::Box measured_;
protected:
	ImageDrawable(const geom::Box& size, const std::string& filename);
	virtual ~ImageDrawable() noexcept = default;
public:
	static Handler<Drawable> create( DrawableFactory& factory, const geom::Box& size, const std::string& repl );
	Handler<gl::Sprite> sprite() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class RepeatDrawable : public Drawable {
private:
	RepeatDrawable(const geom::Box& size, Handler<Drawable> child);
	Handler<Drawable> child_;
public:
	static Handler<Drawable> create( DrawableFactory& factory, const geom::Box& size, const std::string& repl );
	virtual ~RepeatDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

class StretchDrawable : public Drawable {
private:
	StretchDrawable(const geom::Box size, Handler<Drawable> child);
	Handler<Drawable> child_;
public:
	static Handler<Drawable> create( DrawableFactory& factory, const geom::Box& size, const std::string& repl );
	virtual ~StretchDrawable() noexcept = default;
	Handler<Drawable> child() const;
	virtual geom::Box size() const noexcept override;
	virtual void draw(Canvas& canvas, const geom::Area& area, const float depth=0.0f) override;
	virtual std::string toString() const noexcept override;
};

//-------------------------------------------------------------------------------------------------

class DrawableFactory
{
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	typedef std::function<Handler<Drawable>(DrawableFactory&, const geom::Box&, const std::string&)> constructor;
	std::map<std::string, constructor> factories_;
public:
	DrawableFactory(logging::Logger& log);
	~DrawableFactory() = default;
public:
	Handler<Drawable> queryDrawable( const geom::Box& size, const std::string& repl );
};

}}
