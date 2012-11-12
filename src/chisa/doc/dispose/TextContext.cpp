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

#include "../../util/Platform.h"
#include "TextContext.h"
#include <unicode/unistr.h>
#include <unicode/bytestream.h>
#include <cairo/cairo-ft.h>
#include "../../gl/Drawable.h"
#include "../render/RenderObject.h"

namespace chisa {
namespace doc {

TextContext::TextContext(logging::Logger& log, Handler<RenderTree> renderTree)
:log_(log)
,renderTree_(renderTree)
,font_()
,face_(nullptr)
,nullSurface_(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1))
,nullOption_(cairo_font_options_create())
,cairo_(cairo_create(nullSurface_))
{
	this->pushStyle(gl::TextDrawable::Style::Regular);
	this->pushDeco(gl::TextDrawable::Decoration::None);
	this->pushSize(gl::TextDrawable::DefaultFontSize);
	this->pushFont("");
	this->pushColor(gl::Black);
	this->pushBackColor(gl::Transparent);
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

void TextContext::pushStyle(gl::TextDrawable::Style style)
{
	this->styleStack_.push_back(style);
}

gl::TextDrawable::Style TextContext::nowStyle() const
{
	return this->styleStack_.back();
}

void TextContext::popStyle()
{
	this->styleStack_.pop_back();
}

void TextContext::pushSize(float size)
{
	this->sizeStack_.push_back(size);
}

float TextContext::nowSize() const
{
	return this->sizeStack_.back();
}

void TextContext::popSize()
{
	this->sizeStack_.pop_back();
}

void TextContext::pushColor(const gl::Color& c)
{
	this->colorStack_.push_back(c);
}
gl::Color TextContext::nowColor() const
{
	return this->colorStack_.back();
}
void TextContext::popColor()
{
	this->colorStack_.pop_back();
}


void TextContext::pushBackColor(const gl::Color& c)
{
	this->backColorStack_.push_back(c);
}
gl::Color TextContext::nowBackColor() const
{
	return this->backColorStack_.back();
}
void TextContext::popBackColor()
{
	this->backColorStack_.pop_back();
}

void TextContext::pushDeco(gl::TextDrawable::Decoration deco)
{
	this->decoStack_.push_back(deco);
}
gl::TextDrawable::Decoration TextContext::nowDeco() const
{
	return this->decoStack_.back();
}
void TextContext::popDeco()
{
	this->decoStack_.pop_back();
}

void TextContext::pushFont( const std::string& name )
{
	this->fontStack_.push_back(name);
	this->font_ = this->renderTree_->drawableManager()->queryFont(name);
	if(this->face_){
		cairo_font_face_destroy(this->face_);
	}
	gl::Font::RawFaceSession rfs(this->font_);
	this->face_ = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);

}

std::string TextContext::nowFont() const
{
	return this->fontStack_.back();
}

void TextContext::popFont()
{
	if(!this->face_){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Oops. you call \"popFont\" before pushing!");
	}
	cairo_font_face_destroy(this->face_);
	this->fontStack_.pop_back();
	const std::string name = this->fontStack_.back();
	this->font_ = this->renderTree_->drawableManager()->queryFont(name);
	gl::Font::RawFaceSession rfs(this->font_);
	this->face_ = cairo_ft_font_face_create_for_ft_face(rfs.face(),0);
}

TextContext::~TextContext() noexcept
{
	cairo_font_options_destroy(this->nullOption_);
	cairo_destroy(this->cairo_);
	cairo_surface_destroy(this->nullSurface_);
}

geom::Box TextContext::measure(const std::string& strUtf8)
{
	gl::TextDrawable::setupCairo(this->cairo_, this->face_, this->nullOption_, this->nowSize(), this->nowStyle());
	cairo_text_extents_t ext;
	cairo_text_extents(this->cairo_, strUtf8.c_str(), &ext);
	//auto offset = geom::Vector(ext.x_bearing, -ext.y_bearing);
	auto size = geom::Box(ext.x_advance, ext.height+ext.y_advance);
	return size;
}

Handler<TextDrawableObject> TextContext::create(const std::string& strUtf8, Node* parentNode, const float relDepth )
{
	return Handler<TextDrawableObject> ( new TextDrawableObject (
			this->renderTree_,
			parentNode,
			relDepth,
			strUtf8,
			this->nowSize(),
			this->font_,
			this->nowStyle(),
			this->nowDeco(),
			this->nowColor(),
			this->nowBackColor()
	) );
}

}}
