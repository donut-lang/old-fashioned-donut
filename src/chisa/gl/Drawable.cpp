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


#include <cstring>
#include <tarte/String.h>

#include "Drawable.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "Canvas.h"
#include "DrawableManager.h"

namespace chisa {
namespace gl {

float Drawable::width()
{
	return this->size().width();
}
float Drawable::height()
{
	return this->size().height();
}

//-----------------------------------------------------------------------------

ColorDrawable::ColorDrawable(HandlerW<DrawableManager> manager, const geom::Box size, Color const& c)
:Drawable(manager)
,size_(size)
,color_(c)
{

}

Color ColorDrawable::color() const noexcept
{
	return this->color_;
}

void ColorDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth)
{
	if(size_.isSpecified()) {
		geom::Area const orig(ptInRoot, size_);
		geom::Area const masked(ptInRoot+mask.point(), mask.box());
		canvas.fillRect(this->color_, orig.intersect(masked), depth);
	}else{
		canvas.fillRect(this->color_, geom::Area(mask.point()+ptInRoot, mask.box()), depth);
	}
}

Handler<Drawable> ColorDrawable::create( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& repl )
{
	return Handler<Drawable>(new ColorDrawable(manager, size, Color::fromString( repl )));
}

std::string ColorDrawable::toString() const
{
	return ::tarte::format("(ColorDrawable %p size:%s color: %s)", this, this->size_.toString().c_str(), this->color_.toString().c_str());
}

//-----------------------------------------------------------------------------

ImageDrawable::ImageDrawable( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& filename )
:Drawable(manager)
,size_(size)
,filename_(filename)
{
}

Handler<gl::Sprite> ImageDrawable::sprite()
{
	if(!this->sprite_){
		if(Handler<DrawableManager> mgr = this->manager().lock()){
			this->sprite_ = mgr->queryImage(this->filename_);
		}
	}
	return this->sprite_;
}

geom::Box ImageDrawable::size()
{
	Handler<gl::Sprite> spr(this->sprite());
	return spr ? geom::min(geom::Box(spr->size()), this->size_) : size_;
}

void ImageDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth)
{
	if(this->sprite_){
		geom::Area orig(ptInRoot, size_);
		canvas.drawSprite(this->sprite_, ptInRoot, orig.intersect(mask), depth);
	}
}

Handler<Drawable> ImageDrawable::create( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& repl )
{
	return Handler<Drawable>(new ImageDrawable(manager, size, repl));
}

std::string ImageDrawable::toString() const
{
	return ::tarte::format("(ImageDrawable %p size:%s file: %s sprite: %p)", this, this->size_.toString().c_str(), this->filename_.c_str(), this->sprite_.get());
}

//-----------------------------------------------------------------------------

RepeatDrawable::RepeatDrawable(HandlerW<DrawableManager> manager, geom::Box const& size, Handler<Drawable> child)
:Drawable(manager)
,size_(size)
,child_(child)
{

}

Handler<Drawable> RepeatDrawable::child() const
{
	return this->child_;
}

geom::Box RepeatDrawable::size()
{
	return this->size_;
}

void RepeatDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth)
{
	geom::Area const rmask(mask.intersect(geom::Area(geom::ZERO, size_)));
	const float ex = mask.x()+mask.width();
	const float ey = mask.y()+mask.height();

	const float cy = this->child_->height();
	const float cx = this->child_->width();

	for(float dy=0;dy<ey; dy+=cy) {
		for(float dx=0;dx<ex; dx+=cx) {
			geom::Area carea(dx,dy,cx,cy);
			geom::Area cmask(carea.intersect(mask));
			if(!cmask.empty()){
				this->child_->draw(canvas, ptInRoot+geom::Point(dx,dy), cmask, depth);
			}
		}
	}
}

Handler<Drawable> RepeatDrawable::create( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& repl )
{
	return Handler<Drawable>(new RepeatDrawable(manager, size, manager.lock()->queryDrawable(repl, size)));
}

std::string RepeatDrawable::toString() const
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return ::tarte::format("(RepeatDrawable %p size:%s drawable: %s)", this, this->size_.toString().c_str(), childRepl.c_str());
}

//-----------------------------------------------------------------------------

StretchDrawable::StretchDrawable(HandlerW<DrawableManager> manager, const geom::Box size, Handler<Drawable> child)
:Drawable(manager)
,size_(size)
,child_(child)
{

}

Handler<Drawable> StretchDrawable::child() const
{
	return this->child_;
}

geom::Box StretchDrawable::size()
{
	return this->size_;
}

void StretchDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth)
{
	Canvas::AffineScope as(canvas);
	{
		canvas.translate(ptInRoot);
		geom::ScaleVector scale(child_->size() / size_);
		canvas.scale(scale);
		this->child_->draw(canvas, geom::ZERO, mask*scale, depth);
	}
}
Handler<Drawable> StretchDrawable::create( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& repl )
{
	return Handler<Drawable>(new StretchDrawable(manager, size, manager.lock()->queryDrawable(repl, size)));
}

std::string StretchDrawable::toString() const
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return ::tarte::format("(StretchDrawable %p size:%s drawable: %s)", this, this->size_.toString().c_str(), childRepl.c_str());
}

//-----------------------------------------------------------------------------

Handler<Drawable> NullDrawable::create( HandlerW<DrawableManager> manager, geom::Box const& size, std::string const& repl )
{
	return Handler<Drawable>(new NullDrawable(manager, size));
}

std::string NullDrawable::toString() const
{
	return ::tarte::format("(NullDrawable %p size: %s)", this, this->size_.toString().c_str());
}

//-----------------------------------------------------------------------------
const float TextDrawable::DefaultFontSize=16.0f;

TextDrawable::TextDrawable(HandlerW<DrawableManager> manager, std::string const& str, bool vertical, const float size, Handler<Font> font, Style style, Decoration deco, gl::Color const& color, gl::Color const& backColor)
:Drawable(manager)
,str_(str)
,vertical_(vertical)
,fontSize_(size)
,font_(font)
,style_(style)
,deco_(deco)
,color_(color)
,backColor_(backColor)
,font_ascent_(0)
,font_descent_(0)
,font_height_(0)
{
	this->revalidate();
}

void TextDrawable::revalidate()
{
	Handler<DrawableManager> mgr = this->manager().lock();
	if( unlikely(!mgr) ) {
		return;
	}
	std::vector<Handler<BitmapGlyph> > glyphs =
			this->font_->lookupGlyph(this->str_, this->fontSize_, this->font_ascent_, this->font_descent_, this->font_height_);
	{
		int totalX = 0;
		for(Handler<BitmapGlyph> const& glyph : glyphs){
			FT_BitmapGlyph g = glyph->get();
			totalX += g->root.advance.x;
		}
		float const widthf = FLOAT_FROM_16_16(totalX);
		int const width = std::ceil(widthf);
		int const height = std::ceil(font_height_);
		this->size_ = geom::Box(widthf, font_height_);
		this->tsize_ = geom::IntBox(width, height);
		this->sprite_ = mgr->queryRawSprite(ImageFormat::ALPHA, width, height);
	}
	gl::Sprite::Session ss(this->sprite_);
	int nowX = 0;
	int const spriteStride = ss.stride();
	unsigned char* const spriteBuf = ss.data();

	for(Handler<BitmapGlyph> const& glyph : glyphs){
		FT_BitmapGlyph g = glyph->get();
		int const startX = INT_FROM_16_16_FLOOR(nowX)+g->left;

		int const bmpPitch = g->bitmap.pitch;
		int const bmpWidth = g->bitmap.width;

		int const startY = this->font_ascent_-g->top;
		int const endY = g->bitmap.rows+startY;
		int bmpY = 0;
		for(int y=startY;y<endY;++y,++bmpY) {
			unsigned char* const sprBuf = &spriteBuf[y*spriteStride+startX];
			unsigned char* const bmpBuf = &g->bitmap.buffer[bmpPitch*bmpY];
			for(int x=0;x<bmpWidth; ++x) {
				sprBuf[x] = bmpBuf[x];
			}
		}
		nowX += g->root.advance.x;
	}
}

Handler<gl::Sprite> TextDrawable::sprite()
{
	if(!this->sprite_){
		this->revalidate();
	}
	return this->sprite_;
}

Handler<TextDrawable> TextDrawable::create(HandlerW<DrawableManager> manager, std::string const& str, bool vertical, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor)
{
	return Handler<TextDrawable>(new TextDrawable(manager, str, vertical, size, font, style, deco, color, backColor));
}

geom::Box TextDrawable::size()
{
	return this->vertical_ ? this->size_.flip() : this->size_;
}

void TextDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, geom::Area const& mask, const float depth)
{
	if(this->vertical_) {
		Canvas::AffineScope af(canvas);
		canvas.translate(ptInRoot);
		canvas.rotate(90);
		canvas.translate(geom::Point(0, -size_.height()));
		canvas.drawSprite(this->sprite(), geom::ZERO, geom::Area(geom::ZERO, tsize_).intersect(mask.flip()), depth, this->color_);
	}else{
		canvas.drawSprite(this->sprite(), ptInRoot, geom::Area(geom::ZERO, tsize_).intersect(mask), depth, this->color_);
	}
}
void TextDrawable::draw(Canvas& canvas, geom::Point const& ptInRoot, const float depth)
{
	if(this->vertical_) {
		Canvas::AffineScope af(canvas);
		canvas.translate(ptInRoot);
		canvas.rotate(90);
		canvas.translate(geom::Point(0, -this->size_.height()));
		canvas.drawSprite(this->sprite(), geom::ZERO, depth, color_);
	}else{
		canvas.drawSprite(this->sprite(), ptInRoot, depth, color_);
	}
}

std::string TextDrawable::toString() const
{
	return ::tarte::format("(TextDrawable %p str:\"%p\" size: %d)", this, this->str_.c_str(), fontSize_);
}

//-----------------------------------------------------------------------------

}}

