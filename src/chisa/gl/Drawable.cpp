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

#include <cairo/cairo-ft.h>
#include <tarte/String.h>

#include "Drawable.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "internal/CairoUtil.h"
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

void ColorDrawable::draw(Canvas& canvas, geom::Area const& area, const float depth)
{
	canvas.fillRect(this->color_, area.intersect(geom::Area(area.point(), geom::min(area.box(), size_))), depth);
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

void ImageDrawable::draw(Canvas& canvas, geom::Area const& area, const float depth)
{
	if(this->sprite_){
		canvas.drawSprite(this->sprite_, area.point(), geom::Area(geom::ZERO, geom::min(area.box(), this->size())), depth);
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

void RepeatDrawable::draw(Canvas& canvas, geom::Area const& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), geom::min(size_, area.box()))));
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

void StretchDrawable::draw(Canvas& canvas, geom::Area const& area, const float depth)
{
	geom::Area rendered(area.intersect(geom::Area(area.point(), geom::min(area.box(), size_))));
	Canvas::AffineScope as(canvas);
	{
		canvas.translate(area.point());
		float const scaleX = this->child_->width() / rendered.width();
		float const scaleY = this->child_->height() / rendered.height();
		canvas.scale(geom::ScaleVector(scaleX, scaleY));
		this->child_->draw(canvas, geom::Area(geom::ZERO, this->child_->size()), depth);
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
	Font::RawFaceSession session(font_);
	cairo_surface_t* surf = cairo_image_surface_create(CAIRO_FORMAT_A8, 1,1);
	cairo_t* cr = cairo_create(surf);
	cairo_font_options_t* opt = cairo_font_options_create();
	cairo_font_face_t* face = cairo_ft_font_face_create_for_ft_face(session.face(), 0);
	{
		TextDrawable::setupCairo(cr, face, opt, fontSize_, style_);
		cairo_text_extents_t ext;
		cairo_text_extents(cr, str_.c_str(), &ext);
		Font::calcLineInfo(session.face(), fontSize_, this->font_ascent_, this->font_descent_, this->font_height_);
		this->tbearing_ = geom::Vector(ext.x_bearing, ext.y_bearing);
		this->tadvance_ = geom::Vector(ext.x_advance, ext.y_advance);
		this->tsize_ = geom::Box(ext.width, ext.height);
		this->size_ = geom::Box(ext.x_advance, this->font_height_);
		if(this->vertical_){
			this->size_ = this->size_.flip();
		}
	}
	cairo_font_face_destroy(face);
	cairo_font_options_destroy(opt);
	cairo_destroy(cr);
	cairo_surface_destroy(surf);
}

Handler<gl::Sprite> TextDrawable::sprite()
{
	if(this->sprite_){
		return this->sprite_;
	}
	if(Handler<DrawableManager> mgr = this->manager().lock()){
		if(this->vertical_){
			const int width = static_cast<int>(std::ceil(this->tsize_.height()));
			const int height = static_cast<int>(std::ceil(this->tsize_.width()));
			this->sprite_ = mgr->queryRawSprite(width, height);
		}else{
			const int width = static_cast<int>(std::ceil(this->tsize_.width()));
			const int height = static_cast<int>(std::ceil(this->tsize_.height()));
			this->sprite_ = mgr->queryRawSprite(width, height);
		}
	} else {
		return this->sprite_;
	}
#if IS_BIG_ENDIAN
	gl::Sprite::Session ss(spr, gl::Sprite::BufferType::ARGB8);
#else
	gl::Sprite::Session ss(this->sprite_, gl::Sprite::BufferType::BGRA8);
#endif
	gl::Font::RawFaceSession rfs(this->font_);
	{
		cairo_surface_t* surf = cairo_image_surface_create_for_data(ss.data(), CAIRO_FORMAT_ARGB32, ss.width(), ss.height(), ss.stride());
		cairo_t* cr = cairo_create(surf);
		cairo_font_face_t* face = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);
		cairo_font_options_t* opt = cairo_font_options_create();

		//データは使いまわしているので一旦サーフェイスの中身を削除する
		cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
		cairo::setColor(cr, this->backColor_);
		cairo_paint(cr);

		TextDrawable::setupCairo(cr, face, opt, this->fontSize_, this->style_);

		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
		if(this->vertical_){
			cairo_rotate(cr, PI/2.0f);
			cairo_translate(cr, 0, -this->tsize_.height());
		}

		cairo_move_to(cr, -this->tbearing_.x(), -this->tbearing_.y());
		cairo::setColor(cr, this->color_);
		//cairo_show_text(cr, this->str_.c_str());
		cairo_text_path(cr, this->str_.c_str());
		cairo_fill(cr);

		cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
		switch( this->deco_ ){
		case Strike:{
			const float hh = this->tsize_.height()/2;
			cairo_move_to(cr, 0, hh);
			cairo_line_to(cr, this->tsize_.width(), hh);
			cairo_stroke(cr);
			break;
		}
		case Underline: {
			const float width = cairo_get_line_width(cr)/2;
			const float h = this->tsize_.height()-width;
			cairo_move_to(cr, 0, h);
			cairo_line_to(cr, this->tsize_.width(), h);
			cairo_stroke(cr);
			break;
		}
		case None: {
			break;
		}
		default: {
			TARTE_EXCEPTION(Exception, "[BUG] Oops. Invalid decoration: %d", this->deco_);
		}
		}

		cairo_font_options_destroy(opt);
		cairo_font_face_destroy(face);
		cairo_surface_destroy(surf);
		cairo_destroy(cr);
	}
	return this->sprite_;
}

Handler<TextDrawable> TextDrawable::create(HandlerW<DrawableManager> manager, std::string const& str, bool vertical, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor)
{
	return Handler<TextDrawable>(new TextDrawable(manager, str, vertical, size, font, style, deco, color, backColor));
}

geom::Box TextDrawable::size()
{
	return this->size_;
}

void TextDrawable::draw(Canvas& canvas, geom::Area const& area, const float depth)
{
	if(this->vertical_) {
		geom::Distance off(this->font_height_ - (this->font_ascent_+(this->tsize_.height()+this->bearing().y())), this->bearing().x());
		canvas.drawSprite(this->sprite(), area.point()+off, depth);
	}else{
		geom::Distance off(this->bearing().x(), this->font_ascent_+this->bearing().y());
		canvas.drawSprite(this->sprite(), area.point()+off, depth);
	}
}

void TextDrawable::draw(Canvas& canvas, geom::Point const& pt, const float depth)
{
	if(this->vertical_) {
		geom::Distance off(this->font_height_ - (this->font_ascent_+(this->tsize_.height()+this->bearing().y())), this->bearing().x());
		canvas.drawSprite(this->sprite(), pt+off, depth);
	}else{
		geom::Distance off(this->bearing().x(), this->font_ascent_+this->bearing().y());
		canvas.drawSprite(this->sprite(), pt+off, depth);
	}
}

std::string TextDrawable::toString() const
{
	return ::tarte::format("(TextDrawable %p str:\"%p\" size: %d)", this, this->str_.c_str(), fontSize_);
}

void TextDrawable::setupCairo(cairo_t* cairo, cairo_font_face_t* face, cairo_font_options_t* opt, float size, Style style)
{
	cairo_font_options_set_subpixel_order(opt, CAIRO_SUBPIXEL_ORDER_DEFAULT);
	cairo_font_options_set_antialias(opt, CAIRO_ANTIALIAS_DEFAULT);
	cairo_font_options_set_hint_metrics(opt, CAIRO_HINT_METRICS_DEFAULT);
	cairo_font_options_set_hint_style(opt, CAIRO_HINT_STYLE_DEFAULT);
	cairo_set_font_options(cairo, opt);
	cairo_set_font_face(cairo, face);
	cairo_set_font_size(cairo, size);
	//FIXME Italic/Bold対応
}

//-----------------------------------------------------------------------------

}}

