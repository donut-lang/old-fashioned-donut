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

#include "Drawable.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "../gl/Canvas.h"
#include "../util/StringUtil.h"

namespace chisa {
namespace gl {

float Drawable::width() const noexcept
{
	return this->size().width();
}
float Drawable::height() const noexcept
{
	return this->size().height();
}

//-----------------------------------------------------------------------------

ColorDrawable::ColorDrawable(const geom::Box size, const Color& c)
:Drawable(size)
,color_(c)
{

}

Color ColorDrawable::color() const noexcept
{
	return this->color_;
}

void ColorDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	canvas.fillRect(this->color_, area.intersect(geom::Area(area.point(), geom::min(area.box(), this->specSize()))), depth);
}

Handler<Drawable> ColorDrawable::create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new ColorDrawable(size, Color::fromString( repl )));
}

std::string ColorDrawable::toString() const noexcept
{
	return util::format("(ColorDrawable %p size:%s color: %s)", this, this->specSize().toString().c_str(), this->color_.toString().c_str());
}

//-----------------------------------------------------------------------------

ImageDrawable::ImageDrawable( const geom::Box& size, const std::string& filename )
:Drawable(size)
,filename_(filename)
{
}

Handler<gl::Sprite> ImageDrawable::sprite() const
{
	return this->sprite_;
}

geom::Box ImageDrawable::size() const noexcept
{
	return this->sprite_ ? geom::Box(this->sprite_->width(), this->sprite_->height()) : this->specSize();
}

void ImageDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	if(!this->sprite_){
		this->sprite_ = canvas.queryImage(this->filename_);
		this->measured_ = geom::min(this-> specSize(), geom::Box(this->sprite()->size()));
	}
	canvas.drawSprite(this->sprite_, area.point(), geom::Area(geom::ZERO, geom::min(area.box(), this->measured_)), depth);
}

Handler<Drawable> ImageDrawable::create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new ImageDrawable(size, repl));
}

std::string ImageDrawable::toString() const noexcept
{
	return util::format("(ImageDrawable %p size:%s file: %s sprite: %p)", this, this->specSize().toString().c_str(), this->filename_.c_str(), this->sprite_.get());
}

//-----------------------------------------------------------------------------

RepeatDrawable::RepeatDrawable(const geom::Box& size, Handler<Drawable> child)
:Drawable(size)
,child_(child)
{

}

Handler<Drawable> RepeatDrawable::child() const
{
	return this->child_;
}

geom::Box RepeatDrawable::size() const noexcept
{
	return this->child_ ? this->child_->size() : this->specSize();
}

void RepeatDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), geom::min(this->specSize(), area.box()))));
	float y=rendered.y();
	const float mx = rendered.width()+rendered.x();
	const float my = rendered.height()+rendered.y();
	const float cy = this->child_->height();
	const float cx = this->child_->width();
	while(y < my){
		float x=rendered.x();
		while(x < mx){
			this->child_->draw(canvas, geom::Area(x,y, geom::min(cx,mx-x), geom::min(cy,my-y)),depth);
			x+=cy;
		}
		y += cy;
	}
}

Handler<Drawable> RepeatDrawable::create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new RepeatDrawable(size, interpreter.queryDrawable(size, repl)));
}

std::string RepeatDrawable::toString() const noexcept
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return util::format("(RepeatDrawable %p size:%s drawable: %s)", this, this->specSize().toString().c_str(), childRepl.c_str());
}

//-----------------------------------------------------------------------------

StretchDrawable::StretchDrawable(const geom::Box size, Handler<Drawable> child)
:Drawable(size)
,child_(child)
{

}

Handler<Drawable> StretchDrawable::child() const
{
	return this->child_;
}

geom::Box StretchDrawable::size() const noexcept
{
	return this->child_ ? this->child_->size() : this->specSize();
}

void StretchDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), geom::min(area.box(), this->specSize()))));
	Canvas::AffineScope as(canvas);
	{
		canvas.translate(area.point());
		float const scaleX = this->child_->width() / rendered.width();
		float const scaleY = this->child_->height() / rendered.height();
		canvas.scale(geom::ScaleVector(scaleX, scaleY));
		this->child_->draw(canvas, geom::Area(geom::ZERO, this->child_->size()), depth);
	}
}
Handler<Drawable> StretchDrawable::create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new StretchDrawable(size, interpreter.queryDrawable(size, repl)));
}

std::string StretchDrawable::toString() const noexcept
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return util::format("(StretchDrawable %p size:%s drawable: %s)", this, this->specSize().toString().c_str(), childRepl.c_str());
}

//-----------------------------------------------------------------------------

Handler<Drawable> NullDrawable::create( DrawableInterpreter& interpreter, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new NullDrawable(size));
}

std::string NullDrawable::toString() const noexcept
{
	return util::format("(NullDrawable %p size: %s)", this, this->specSize().toString().c_str());
}

//-----------------------------------------------------------------------------

DrawableInterpreter::DrawableInterpreter(logging::Logger& log)
:log_(log)
{
	this->factories_.insert(std::make_pair("stretch:", StretchDrawable::create));
	this->factories_.insert(std::make_pair("repeat:", RepeatDrawable::create));
	this->factories_.insert(std::make_pair("image:", ImageDrawable::create));
	this->factories_.insert(std::make_pair("color:", ColorDrawable::create));
	this->factories_.insert(std::make_pair("none:", NullDrawable::create));
}
Handler<Drawable> DrawableInterpreter::queryDrawable( const geom::Box& size, const std::string& repl )
{
	for(std::pair<std::string, constructor> p : this->factories_){
		if(util::startsWith(repl, p.first)){
			std::string const left = repl.substr(p.first.size());
			return p.second(*this, size, left);
		}
	}
	this->log().w("DrawableFactory", "oops. Invalid repl: %s", repl.c_str());
	return NullDrawable::create(*this, size, repl);
}

}}
