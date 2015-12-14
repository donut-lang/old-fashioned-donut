/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "../Font.hpp"

namespace chisa {
namespace gl {

BitmapGlyph::BitmapGlyph(FT_BitmapGlyph glyph)
:glyph_(glyph)
{

}
BitmapGlyph::~BitmapGlyph() noexcept
{
	FT_Done_Glyph(reinterpret_cast<FT_Glyph>(this->glyph_));
	this->glyph_ = nullptr;
}

}}
