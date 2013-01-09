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

#include "../Font.h"
#include <cmath>
#include <tarte/FileSystem.h>

namespace chisa {
namespace gl {
namespace internal {

static FT_Error face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface )
{
    *aface = (FT_Face)face_id;
    return 0;
}

FreeType::FreeType()
:library_(nullptr)
,cache_(nullptr)
,cmap_(nullptr)
,image_(nullptr)
{
	if(FT_Init_FreeType(&this->library_) != 0){
		TARTE_EXCEPTION(Exception, "[BUG] Failed to init Freetype.");
	}
	if(FTC_Manager_New(this->library_, 16, 0, 1024*1024*10, face_requester, nullptr, &this->cache_ )){
		FT_Done_FreeType(this->library_);
		TARTE_EXCEPTION(Exception, "[BUG] Failed to init cache manager.");
	}
	if(FTC_CMapCache_New(this->cache_, &this->cmap_)) {
		FTC_Manager_Done(this->cache_);
		FT_Done_FreeType(this->library_);
		TARTE_EXCEPTION(Exception, "[BUG] Failed to init cmap cache.");
	}
	if(FTC_ImageCache_New(this->cache_, &this->image_)) {
		FTC_Manager_Done(this->cache_);
		FT_Done_FreeType(this->library_);
		TARTE_EXCEPTION(Exception, "[BUG] Failed to init image cache.");
	}
}

FreeType::~FreeType() noexcept
{
	FTC_Manager_Done(this->cache_);
	FT_Done_FreeType(this->library_);
	this->cmap_ = nullptr;
	this->image_ = nullptr;
	this->cache_ = nullptr;
	this->library_ = nullptr;
}
void FreeType::removeFace(FT_Face face)
{
	FTC_Manager_RemoveFaceID(this->cache_, face);
}

unsigned int FreeType::lookupGlyphIndex(Font& font, unsigned int ucs4)
{
	Font::RawFaceSession rfs(font);
	FT_Face face = rfs.face();
	return FTC_CMapCache_Lookup(this->cmap_, face, font.unicodeCharmapIndex(), ucs4);
}

std::vector<Handler<BitmapGlyph> > FreeType::lookupBitmap(Font& font, float size, std::vector<unsigned int> const& ucs4, float& ascent, float& descent, float& height)
{
	Font::RawFaceSession rfs(font);
	FT_Face face = rfs.face();
	FT_Face face__;
	FTC_ScalerRec scaler;
	{
		scaler.face_id = face;
		scaler.width = 0;
		scaler.height = FLOAT_TO_26_6( size );
		scaler.pixel = 0;
		scaler.x_res=72; //XXX
		scaler.y_res=72; //XXX
	}

	FTC_Manager_LookupFace(this->cache_, face, &face__);
	{
		FT_Size size;
		FTC_Manager_LookupSize(this->cache_, &scaler, &size);
		ascent = FLOAT_FROM_26_6(size->metrics.ascender);
		descent = FLOAT_FROM_26_6(size->metrics.descender);
		height = FLOAT_FROM_26_6(size->metrics.height);
	}
	std::vector<Handler<BitmapGlyph> > ret;
	ret.reserve(ucs4.size());
	for(unsigned int const& u : ucs4) {
		unsigned int const gindex = FTC_CMapCache_Lookup(this->cmap_, face, font.unicodeCharmapIndex(), u);
		FT_Glyph glyph;
		{
			FT_Glyph glyph_orig;
			FTC_ImageCache_LookupScaler(this->image_, &scaler, 0, gindex, &glyph_orig, nullptr);
			FT_Glyph_Copy(glyph_orig, &glyph);
		}
		if(glyph->format != FT_GLYPH_FORMAT_BITMAP ) {
			FT_Vector_ vec;
			vec.x = 0;
			vec.y = 0;
			FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &vec, 1);
		}
		ret.push_back( Handler<BitmapGlyph>(new BitmapGlyph(reinterpret_cast<FT_BitmapGlyph>(glyph))) );
	}
	return ret;
}


}}}

