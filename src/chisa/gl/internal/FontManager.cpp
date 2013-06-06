/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "../Font.h"
#include <cinamo/FileSystem.h>

namespace chisa {
namespace gl {

namespace internal {

static std::string TAG("Font");

FontManager::FontManager(Logger& log, std::string const& fontdir)
:log_(log)
,fontdir_(fontdir)
,freetype_( new FreeType )
{
	this->defaultFont_ = Handler<Font>( this->seachDefaultFont() );
}

FontManager::~FontManager() noexcept
{
	decltype(this->unusedFonts_)().swap(this->unusedFonts_);
	this->defaultFont_.reset();
}

Handler<Font> FontManager::queryFont(std::string const& name)
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

Font* FontManager::searchFont( std::string const& name )
{
	if(name.empty()){
		return nullptr;
	}
	FT_Face face = nullptr;
	std::string family;
	std::string style;
	Font::analyzeFontName(name, family, style);
	std::vector<std::string> files(file::enumFiles(this->fontdir_));
	for(std::string const& fname : files){
		int face_idx=0;
		while(FT_New_Face(this->freetype_->raw(), fname.c_str(), (face_idx++), &face) == 0){
			if(face->family_name && family == std::string(face->family_name) &&
					(!style.empty() && face->style_name) && style==std::string(face->style_name) ){
				return new Font(this, this->freetype_, face);
			}
			FT_Done_Face(face);
		}
	}
	return nullptr;
}

Font* FontManager::seachDefaultFont()
{
	FT_Face face;
	std::vector<std::string> files(file::enumFiles(this->fontdir_));
	for(std::string const& fname : files){
		if(FT_New_Face(this->freetype_->raw(), fname.c_str(), 0, &face) == 0){
			return new Font(this, this->freetype_, face);
		}else{
			this->log().e(TAG, "Failed to open font: %s", fname.c_str());
		}
	}
	CINAMO_EXCEPTION(Exception, "[BUG] Failed open default font: %s", this->fontdir_.c_str());
}

void FontManager::backFont(Font* font)
{
	this->unusedFonts_.push_back(Handler<Font>(font));
	while(this->unusedFonts_.size() >= FontManager::MaxUnusedFonts){
		this->unusedFonts_.pop_front();
	}
}

}}}

