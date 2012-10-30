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

#include "Font.h"
#include "../util/FileUtil.h"
#include "../util/StringUtil.h"

namespace chisa {
namespace gl {

static std::string TAG("Font");

FontManager::FontManager(logging::Logger& log, const std::string& fontdir)
:HANDLER_KLASS_WEAK_INIT
,log_(log)
,fontdir_(fontdir)
,freetype_(nullptr)
{
	if(FT_Init_FreeType(&this->freetype_) != 0){
		throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed to init Freetype.");
	}
	this->defaultFont_ = Handler<Font>( this->seachDefaultFont() );
}

FontManager::~FontManager() noexcept
{
	decltype(this->unusedFonts_)().swap(this->unusedFonts_);
	this->defaultFont_.reset();
	FT_Done_FreeType(this->freetype_);
	this->freetype_ = nullptr;
}

void FontManager::onFree()
{
	HANDLER_KLASS_WEAK_DEAD;
}

Handler<Font> FontManager::queryFont(const std::string& name)
{
	if(name.empty()){
		return this->defaultFont_;
	}
	std::string family;
	std::string style;
	Font::analyzeFontName(name, family, style);
	for(auto it = this->unusedFonts_.begin(); it != this->unusedFonts_.end(); ++it){
		Handler<Font> font(*it);
		if(font->family() == family && (style.empty() || font->style() == style)){
			this->unusedFonts_.erase(it);
			return font;
		}
	}
	Handler<Font> const font( searchFont( name ) );
	if( font ){
		return font;
	}
	this->log().w(TAG, "Failed to search font: %s", name.c_str());
	return this->defaultFont_;
}

Font* FontManager::searchFont( const std::string& name )
{
	if(name.empty()){
		return nullptr;
	}
	FT_Face face = nullptr;
	std::string family;
	std::string style;
	Font::analyzeFontName(name, family, style);
	std::set<std::string> files;
	util::file::enumFiles(this->fontdir_, files);
	for(const std::string& fname : files){
		unsigned int face_idx=0;
		unsigned int face_max = 2;
		while(FT_New_Face(this->freetype_, fname.c_str(), (face_idx++), &face) == 0 && face_idx < face_max){
			face_max = face->num_faces;
			if(face->family_name && family == std::string(face->family_name) &&
					(!style.empty() && face->style_name) && style==std::string(face->style_name) ){
				return new Font(this, face);
			}
			FT_Done_Face(face);
		}
	}
	return nullptr;
}

Font* FontManager::seachDefaultFont()
{
	FT_Face face;
	std::set<std::string> files;
	util::file::enumFiles(this->fontdir_, files);
	for(const std::string& fname : files){
		if(FT_New_Face(this->freetype_, fname.c_str(), 0, &face) == 0){
			return new Font(this, face);
		}else{
			this->log().e(TAG, "Failed to open font: %s", fname.c_str());
		}
	}
	throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed open default font: %s", this->fontdir_.c_str());
}

void FontManager::backFont(Font* font)
{
	this->unusedFonts_.push_back(Handler<Font>(font));
	while(this->unusedFonts_.size() >= FontManager::MaxUnusedFonts){
		this->unusedFonts_.pop_front();
	}
}

Font::Font(FontManager* parent, FT_Face face)
:HANDLER_KLASS_INIT
,parent_(parent)
,face_(face)
,locked_(false)
{
}

Font::~Font()
{
	if(!this->parent_.expired()){
		FT_Done_Face(this->face_);
	}
	this->face_ = nullptr;
}

void Font::onFree()
{
	if(Handler<FontManager> p = this->parent_.lock()){
		p->backFont(this);
	}else{
		delete this;
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

void Font::analyzeFontName(const std::string& name, std::string& family, std::string& style) noexcept
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
