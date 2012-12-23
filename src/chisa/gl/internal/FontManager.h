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

#pragma once
#include <string>
#include <deque>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <tarte/Logger.h>
#include <tarte/Handler.h>
#include <tarte/ClassUtil.h>

namespace chisa {
using namespace tarte;
namespace gl {
class Font;

namespace internal {
class FreeType : public HandlerBody<FreeType> {
private:
	FT_Library library_;
public:
	FreeType();
	~FreeType() noexcept;
public:
	FT_Library raw() const noexcept { return this->library_; };
	inline bool onFree() noexcept { return false; };
};

class FontManager : public HandlerBody<FontManager> {
	DEFINE_MEMBER_REF(private, Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, fontdir);
private:
	static constexpr std::size_t MaxUnusedFonts = 100;
	Handler<internal::FreeType> freetype_;
	std::deque<Handler<Font> > unusedFonts_;
	Handler<Font> defaultFont_;
public:
	FontManager(Logger& log, std::string const& fontdir);
	Handler<Font> queryFont(std::string const& name = std::string());
private:
	~FontManager() noexcept;
private:
	Font* searchFont( std::string const& name );
	Font* seachDefaultFont();
public:
	inline bool onFree() noexcept { return false; };
	void backFont(Font* font);
};


}}}
