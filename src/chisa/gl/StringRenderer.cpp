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

namespace chisa {
namespace gl {

StringRenderer::StringRenderer()
:nullSurface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,cairo_(cairo_create(nullSurface_))
,style_(StringRenderer::Style::Regular)
,size_(StringRenderer::DefaultSize)
{
	this->style(this->style_);
	this->size(this->size_);
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

void StringRenderer::style(Style style)
{
	switch(style)
	{
	case Regular:
		break;
	case Bold:
		break;
	case Italic:
		break;
	case ItalicBold:
		break;
	}
	this->style_ = style;
}

StringRenderer::Style StringRenderer::style()
{
	return this->style_;
}

void StringRenderer::size(float size)
{
	this->size_ = size;
	cairo_set_font_size(this->cairo_, size);
}
float StringRenderer::size()
{
	return this->size_;
}

StringRenderer::~StringRenderer()
{
	cairo_destroy(this->cairo_);
	cairo_surface_destroy(this->nullSurface_);
}

StringRenderer::Command StringRenderer::measure(const std::string& strUtf8)
{
	cairo_text_extents_t ext;
	cairo_text_extents(this->cairo_, strUtf8.c_str(), &ext);
	auto offset = geom::Vector(ext.x_bearing, -ext.y_bearing);
	auto size = geom::Box(ext.x_advance, ext.height+ext.y_advance);
	return StringRenderer::Command(this->style(), this->size(), strUtf8, geom::Area(offset, size));
}

StringRenderer::Command StringRenderer::calcMaximumStringLength(const std::string& ostr, const float limit, size_t beginInUtf8, size_t endInUtf8)
{
	UnicodeString str(UnicodeString::fromUTF8(ostr));
	const size_t len = ostr.length();
	char* const buf = new char[len+1];
	CheckedArrayByteSink sink(buf, len+1);

	std::size_t const beg = beginInUtf8;
	std::size_t const end = endInUtf8 == 0 ? static_cast<size_t>(str.length()) : endInUtf8;

	{ //とりあえず全部で試してみる
		sink.Reset();
		str.tempSubStringBetween(beg, end).toUTF8(sink);
		StringRenderer::Command cmdTry = this->measure(buf);
		if(cmdTry.width() <= limit){
			return cmdTry;
		}
	}

	std::size_t min = beg;
	std::size_t max = end;
	Command cmd;

	while(min+1 < max){
		std::size_t center = (min+max)/2;
		sink.Reset();
		str.tempSubStringBetween(beg, center).toUTF8(sink);
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

gl::Handler<gl::RawSprite> StringRenderer::Command::renderString(gl::Canvas& cv) const
{
	gl::Handler<gl::RawSprite> spr = cv.queryRawSprite(static_cast<int>(this->area().width()), static_cast<int>(this->area().height()));
	gl::RawSprite::Session ss(spr);
	{
		cairo_surface_t* surf = cairo_image_surface_create_for_data(ss.data(), CAIRO_FORMAT_ARGB32, ss.width(), ss.height(), ss.stride());
		cairo_t* cr = cairo_create(surf);

		//データは使いまわしているので一旦サーフェイスの中身を削除する
		cairo_set_source_rgba(cr, 0, 0, 0, 0);
		cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
		cairo_paint(cr);

		cairo_set_font_size(cr, this->size_);

		cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
		cairo_move_to(cr, this->area().x(), this->area().y());
		if(this->vertical()){
			cairo_rotate(cr, 90.0f);
		}

		cairo_set_source_rgba(cr, 1,1,1,1);
		cairo_show_text(cr, this->str().c_str());
		//cairo_text_path(cr, this->str().c_str());
		//cairo_fill(cr);

		cairo_destroy(cr);
	}
	return spr;
}


}}
