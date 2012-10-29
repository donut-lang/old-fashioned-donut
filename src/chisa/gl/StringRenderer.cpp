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

#include "StringRenderer.h"
#include "../geom/Vector.h"
#include "../gl/Canvas.h"
#include "../logging/Exception.h"
#include <cfloat>
#include <cstring>
#include <unicode/unistr.h>
#include <unicode/bytestream.h>
#include <cairo/cairo-ft.h>

namespace chisa {
namespace gl {

const float StringRenderer::DefaultFontSize=16.0f;

StringRenderer::StringRenderer(Handler<gl::FontManager> fontManager)
:fontManager_(fontManager)
,font_()
,face_(nullptr)
,nullSurface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,nullOption_(cairo_font_options_create())
,cairo_(cairo_create(nullSurface_))
{
	this->pushStyle(StringRenderer::Style::Regular);
	this->pushSize(StringRenderer::DefaultFontSize);
	this->pushFont("");
	{
		cairo_status_t st = cairo_surface_status(this->nullSurface_);
		if(st != CAIRO_STATUS_SUCCESS){
			throw logging::Exception(__FILE__, __LINE__, "Oops. Failed to create cairo surface. Error code: %d", st);
		}
	}
	{
		cairo_status_t st = cairo_status(this->cairo_);
		if(st != CAIRO_STATUS_SUCCESS){
			throw logging::Exception(__FILE__, __LINE__, "Oops. Failed to create cairo. Error code: %d", st);
		}
	}
}

void StringRenderer::pushStyle(Style style)
{
	this->styleStack_.push_back(style);
}

StringRenderer::Style StringRenderer::nowStyle() const
{
	return this->styleStack_.back();
}

void StringRenderer::popStyle()
{
	this->styleStack_.pop_back();
}

void StringRenderer::pushSize(float size)
{
	this->sizeStack_.push_back(size);
}

float StringRenderer::nowSize() const
{
	return this->sizeStack_.back();
}

void StringRenderer::popSize()
{
	this->sizeStack_.pop_back();
}

void StringRenderer::pushColor(const Color& c)
{
	this->colorStack_.push_back(c);
}
Color StringRenderer::nowColor() const
{
	return this->colorStack_.back();
}
void StringRenderer::popColor()
{
	this->colorStack_.pop_back();
}


void StringRenderer::pushFont( const std::string& name )
{
	this->fontStack_.push_back(name);
	this->font_ = this->fontManager_->queryFont(name);
	if(this->face_){
		cairo_font_face_destroy(this->face_);
	}
	gl::Font::RawFaceSession rfs(this->font_);
	this->face_ = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);

}

std::string StringRenderer::nowFont() const
{
	return this->fontStack_.back();
}

void StringRenderer::popFont()
{
	if(!this->face_){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. you call \"popFont\" before pushing!");
	}
	cairo_font_face_destroy(this->face_);
	this->fontStack_.pop_back();
	const std::string name = this->fontStack_.back();
	this->font_ = this->fontManager_->queryFont(name);
	gl::Font::RawFaceSession rfs(this->font_);
	this->face_ = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);
}

StringRenderer::~StringRenderer() noexcept
{
	cairo_font_options_destroy(this->nullOption_);
	cairo_destroy(this->cairo_);
	cairo_surface_destroy(this->nullSurface_);
}

StringRenderer::Command StringRenderer::measure(const std::string& strUtf8)
{
	StringRenderer::setupCairo(this->cairo_, this->face_, this->nullOption_, this->nowSize(), this->nowStyle());
	cairo_text_extents_t ext;
	cairo_text_extents(this->cairo_, strUtf8.c_str(), &ext);
	auto offset = geom::Vector(ext.x_bearing, -ext.y_bearing);
	auto size = geom::Box(ext.x_advance, ext.height+ext.y_advance);
	return StringRenderer::Command(this->font_, this->nowStyle(), this->nowSize(), strUtf8, geom::Area(offset, size));
}

StringRenderer::Command StringRenderer::calcMaximumStringLength(const std::string& ostr, const float limit, size_t beginInUtf8, size_t endInUtf8)
{
	UnicodeString str(UnicodeString::fromUTF8(ostr.substr(beginInUtf8, (endInUtf8 == 0 ? static_cast<size_t>(ostr.length()) : endInUtf8)-beginInUtf8)));
	const size_t len = ostr.length();
	char* const buf = new char[len+1];
	CheckedArrayByteSink sink(buf, len+1);

	{ //とりあえず全部で試してみる
		sink.Reset();
		str.toUTF8(sink);
		StringRenderer::Command cmdTry = this->measure(buf);
		if(cmdTry.width() <= limit){
			return cmdTry;
		}
	}

	int min = 0;
	int max = str.length();
	Command cmd;

	while(min+1 < max){
		std::size_t center = (min+max)/2;
		sink.Reset();
		str.tempSubStringBetween(0, center).toUTF8(sink);
		Command cmdTry = this->measure(buf);
		if(cmdTry.width() <= limit){
			cmd = cmdTry;
			min = center;
		}else{
			max = center;
		}
	}
	delete [] buf;
	return cmd;
}

Handler<gl::RawSprite> StringRenderer::Command::renderString(gl::Canvas& cv) const
{
	Handler<gl::RawSprite> spr = cv.queryRawSprite(static_cast<int>(std::ceil(this->area().width())), static_cast<int>(std::ceil(this->area().height())));
	gl::RawSprite::Session ss(spr);
	gl::Font::RawFaceSession rfs(this->font_);
	{
		cairo_surface_t* surf = cairo_image_surface_create_for_data(ss.data(), CAIRO_FORMAT_ARGB32, ss.width(), ss.height(), ss.stride());
		cairo_t* cr = cairo_create(surf);
		cairo_font_face_t* face = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);
		cairo_font_options_t* opt = cairo_font_options_create();

		//データは使いまわしているので一旦サーフェイスの中身を削除する
		cairo_set_source_rgba(cr, 0, 0, 0, 0);
		cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
		cairo_paint(cr);

		StringRenderer::setupCairo(cr, face, opt, this->size_, this->style_);

		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
		cairo_move_to(cr, this->area().x(), this->area().y());
		if(this->vertical()){
			cairo_rotate(cr, 90.0f);
		}

		cairo_set_source_rgba(cr, 1,1,1,1);
		//cairo_show_text(cr, this->str().c_str());
		cairo_text_path(cr, this->str().c_str());
		cairo_fill(cr);
		cairo_font_options_destroy(opt);
		cairo_font_face_destroy(face);
		cairo_surface_destroy(surf);
		cairo_destroy(cr);
	}
	return spr;
}

void StringRenderer::setupCairo(cairo_t* cairo, cairo_font_face_t* face, cairo_font_options_t* opt, float size, Style style)
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

}}
