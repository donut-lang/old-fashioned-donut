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

#include <tarte/FileSystem.h>
#include <tarte/String.h>

#include "Font.h"

namespace chisa {
namespace gl {


Font::Font(internal::FontManager* parent, Handler<internal::FreeType> freetype, FT_Face face)
:parent_(parent)
,freetype_(freetype)
,face_(face)
,locked_(false)
{
}

Font::~Font() noexcept
{
	FT_Done_Face(this->face_);
	this->face_ = nullptr;
}

bool Font::onFree()
{
	if(Handler<internal::FontManager> p = this->parent_.lock()){
		p->backFont(this);
		return true;
	}else{
		return false;
	}
}

std::string Font::family() const noexcept
{
	FT_String* family = this->face_->family_name;
	if(family){
		return std::string(family);
	}else{
		return std::string();
	}
}
std::string Font::style() const noexcept
{
	FT_String* style = this->face_->style_name;
	if ( style ) {
		return std::string( style );
	}
	else {
		return std::string();
	}
}

#define FLOAT_TO_26_6(d) ((FT_F26Dot6)((d) * 64.0))
#define FLOAT_FROM_26_6(t) ((float)(t) / 64.0)

void Font::calcLineInfo(FT_Face face, float const& fontSize, float& ascent, float& descent, float& height)
{
	FT_Size_RequestRec rec;
	rec.type = FT_SIZE_REQUEST_TYPE_NOMINAL;
	rec.height = FLOAT_TO_26_6(fontSize);
	rec.width = FLOAT_TO_26_6(fontSize);
	rec.horiResolution = 0;
	rec.vertResolution = 0;
	FT_Request_Size(face, &rec);

	FT_Size_Metrics const& metrics = face->size->metrics;
	ascent = FLOAT_FROM_26_6(metrics.ascender);
	descent = FLOAT_FROM_26_6(metrics.descender);
	height = FLOAT_FROM_26_6(metrics.height);
}

void Font::calcCharInfo(FT_Face face, float const& fontSize, float& max_width, float& max_height)
{
	FT_Size_RequestRec rec;
	rec.type = FT_SIZE_REQUEST_TYPE_NOMINAL;
	rec.height = FLOAT_TO_26_6(fontSize);
	rec.width = FLOAT_TO_26_6(fontSize);
	rec.horiResolution = 0;
	rec.vertResolution = 0;
	FT_Request_Size(face, &rec);

	FT_Size_Metrics const& metrics = face->size->metrics;
	max_width = FLOAT_FROM_26_6(metrics.max_advance);
	max_height = FLOAT_FROM_26_6(metrics.height);
}

void Font::analyzeFontName(std::string const& name, std::string& family, std::string& style) noexcept
{
	std::string::size_type const idx = name.find(':');
	if(idx == std::string::npos){
		family=name;
		style="";
	}else{
		family = name.substr(0, idx);
		std::string::size_type const last = name.find(':', idx+1);
		style = name.substr(idx+1, last-idx-1);
	}
}

}}
