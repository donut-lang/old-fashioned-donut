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
#include "../Handler.h"
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace chisa {
namespace gl {
namespace internal {
class FreeType : public HandlerBody<FreeType> {
private:
	FT_Library library_;
public:
	FreeType();
	~FreeType() noexcept;
public:
	FT_Library raw() const noexcept { return this->library_; };
	void onFree() noexcept;
};

}

class FontManager;

class Font : public HandlerBody<Font>
{
public:
	enum Flag {
		Italic = 0,
		Bold = 1
	};
private:
	HandlerW<FontManager> parent_;
	Handler<internal::FreeType> freetype_;
	FT_Face face_;
	DEFINE_MEMBER(private, private, bool, locked);
public:
	Font(FontManager* parent, Handler<internal::FreeType> freetype, FT_Face face);
	virtual ~Font() noexcept;
public:
	std::string family() const noexcept;
	std::string style() const noexcept;
public:
	static void analyzeFontName(const std::string& name, std::string& family, std::string& style) noexcept;
public:
	class RawFaceSession {
		DISABLE_COPY_AND_ASSIGN(RawFaceSession);
		STACK_OBJECT(RawFaceSession);
	private:
		Handler<Font> parent_;
	public:
		RawFaceSession(Handler<Font> font):parent_(font){
			if(this->parent_->locked()){
				throw logging::Exception(__FILE__, __LINE__, "[BUG] Failed to lock Font.");
			}
			this->parent_->locked(true);
		};
		~RawFaceSession() noexcept{
			this->parent_->locked(false);
		}
		FT_Face face() const noexcept { return parent_->face_; };
	};
public:
	void onFree();
};

class FontManager : public HandlerBody<FontManager> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, fontdir);
private:
	static constexpr std::size_t MaxUnusedFonts = 100;
	Handler<internal::FreeType> freetype_;
	std::deque<Handler<Font> > unusedFonts_;
	Handler<Font> defaultFont_;
public:
	FontManager(logging::Logger& log, const std::string& fontdir);
	Handler<Font> queryFont(const std::string& name = std::string());
private:
	~FontManager() noexcept;
private:
	Font* searchFont( const std::string& name );
	Font* seachDefaultFont();
public:
	void onFree() noexcept;
	void backFont(Font* font);
};

}}
