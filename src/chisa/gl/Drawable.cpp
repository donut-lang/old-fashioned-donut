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
#include "Drawable.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "../util/StringUtil.h"
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

ColorDrawable::ColorDrawable(HandlerW<DrawableManager> manager, const geom::Box size, const Color& c)
:Drawable(manager)
,size_(size)
,color_(c)
{

}

Color ColorDrawable::color() const noexcept
{
	return this->color_;
}

void ColorDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	canvas.fillRect(this->color_, area.intersect(geom::Area(area.point(), geom::min(area.box(), size_))), depth);
}

Handler<Drawable> ColorDrawable::create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new ColorDrawable(manager, size, Color::fromString( repl )));
}

std::string ColorDrawable::toString() const
{
	return util::format("(ColorDrawable %p size:%s color: %s)", this, this->size_.toString().c_str(), this->color_.toString().c_str());
}

//-----------------------------------------------------------------------------

ImageDrawable::ImageDrawable( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& filename )
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

void ImageDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	if(this->sprite_){
		canvas.drawSprite(this->sprite_, area.point(), geom::Area(geom::ZERO, geom::min(area.box(), this->size())), depth);
	}
}

Handler<Drawable> ImageDrawable::create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new ImageDrawable(manager, size, repl));
}

std::string ImageDrawable::toString() const
{
	return util::format("(ImageDrawable %p size:%s file: %s sprite: %p)", this, this->size_.toString().c_str(), this->filename_.c_str(), this->sprite_.get());
}

//-----------------------------------------------------------------------------

RepeatDrawable::RepeatDrawable(HandlerW<DrawableManager> manager, const geom::Box& size, Handler<Drawable> child)
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

void RepeatDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
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

Handler<Drawable> RepeatDrawable::create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new RepeatDrawable(manager, size, manager.lock()->queryDrawable(repl, size)));
}

std::string RepeatDrawable::toString() const
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return util::format("(RepeatDrawable %p size:%s drawable: %s)", this, this->size_.toString().c_str(), childRepl.c_str());
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

void StretchDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
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
Handler<Drawable> StretchDrawable::create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new StretchDrawable(manager, size, manager.lock()->queryDrawable(repl, size)));
}

std::string StretchDrawable::toString() const
{
	std::string const childRepl(this->child() ? this->child()->toString() : "none");
	return util::format("(StretchDrawable %p size:%s drawable: %s)", this, this->size_.toString().c_str(), childRepl.c_str());
}

//-----------------------------------------------------------------------------

Handler<Drawable> NullDrawable::create( HandlerW<DrawableManager> manager, const geom::Box& size, const std::string& repl )
{
	return Handler<Drawable>(new NullDrawable(manager, size));
}

std::string NullDrawable::toString() const
{
	return util::format("(NullDrawable %p size: %s)", this, this->size_.toString().c_str());
}

//-----------------------------------------------------------------------------
const float TextDrawable::DefaultFontSize=16.0f;

TextDrawable::TextDrawable(HandlerW<DrawableManager> manager, const std::string& str, bool vertical, const float size, Handler<Font> font, Style style, Decoration deco, const gl::Color& color, const gl::Color& backColor)
:Drawable(manager)
,str_(str)
,vertical_(vertical)
,size_(size)
,font_(font)
,style_(style)
,deco_(deco)
,color_(color)
,backColor_(backColor)
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
		TextDrawable::setupCairo(cr, face, opt, size_, style_);
		cairo_text_extents_t ext;
		cairo_text_extents(cr, str_.c_str(), &ext);
		auto offset = geom::Vector(ext.x_bearing, -ext.y_bearing);
		auto size = geom::Box(ext.x_advance+ext.x_bearing, ext.height+ext.y_advance);
		this->renderInfo_ = geom::Area(offset, size);
		if(this->vertical_){
			this->renderInfo_ = this->renderInfo_.flip();
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
		const int width = static_cast<int>(std::ceil(this->renderInfo_.width()));
		const int height = static_cast<int>(std::ceil(this->renderInfo_.height()));
		this->sprite_ = mgr->queryRawSprite(width, height);
	}else{
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
		cairo::setColor(cr, gl::Color(1,1,1,1));
		cairo_paint(cr);

		TextDrawable::setupCairo(cr, face, opt, this->size_, this->style_);

		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
		cairo_move_to(cr, this->renderInfo_.x(), this->renderInfo_.y());
		if(this->vertical_){
			cairo_rotate(cr, 90.0f);
		}

		cairo::setColor(cr, this->color_);
		//cairo_show_text(cr, this->str_.c_str());
		cairo_text_path(cr, this->str_.c_str());
		cairo_fill(cr);

		switch( this->deco_ ){
		case Strike:{
			const float hh = this->renderInfo_.height()/2;
			cairo_move_to(cr, 0, hh);
			cairo_move_to(cr, this->renderInfo_.width(), hh);
			cairo_stroke(cr);
			break;
		}
		case Underline: {
			const float h = this->renderInfo_.height();
			cairo_move_to(cr, 0, h);
			cairo_line_to(cr, this->renderInfo_.width(), h);
			cairo_stroke(cr);
			break;
		}
		case None: {
			break;
		}
		default: {
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Invalid decoration: %d", this->deco_);
		}
		}

		cairo_font_options_destroy(opt);
		cairo_font_face_destroy(face);
		cairo_surface_destroy(surf);
		cairo_destroy(cr);
	}
	return this->sprite_;
}

Handler<TextDrawable> TextDrawable::create(HandlerW<DrawableManager> manager, const std::string& str, bool vertical, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, const gl::Color& color, const gl::Color& backColor)
{
	return Handler<TextDrawable>(new TextDrawable(manager, str, vertical, size, font, style, deco, color, backColor));
}

geom::Box TextDrawable::size()
{
	return this->renderInfo_.box();
}

void TextDrawable::draw(Canvas& canvas, const geom::Area& area, const float depth)
{
	canvas.drawSprite(this->sprite(), area.point(), depth);
}

std::string TextDrawable::toString() const
{
	return util::format("(TextDrawable %p str:\"%p\" size: %d)", this, this->str_.c_str(), size_);
}

void TextDrawable::setupCairo(cairo_t* cairo, cairo_font_face_t* face, cairo_font_options_t* opt, float size, Style style)
{
	cairo_font_options_set_subpixel_order(opt, CAIRO_SUBPIXEL_ORDER_RGB);
	cairo_font_options_set_antialias(opt, CAIRO_ANTIALIAS_DEFAULT);
	cairo_font_options_set_hint_metrics(opt, CAIRO_HINT_METRICS_ON);
	cairo_font_options_set_hint_style(opt, CAIRO_HINT_STYLE_MEDIUM);
//	cairo_font_options_set_subpixel_order(opt, CAIRO_SUBPIXEL_ORDER_DEFAULT);
//	cairo_font_options_set_antialias(opt, CAIRO_ANTIALIAS_NONE);
//	cairo_font_options_set_hint_metrics(opt, CAIRO_HINT_METRICS_OFF);
//	cairo_font_options_set_hint_style(opt, CAIRO_HINT_STYLE_NONE);
	cairo_set_font_options(cairo, opt);
	cairo_set_font_face(cairo, face);
	cairo_set_font_size(cairo, size);
	//FIXME Italic/Bold対応
}

//-----------------------------------------------------------------------------

}}

