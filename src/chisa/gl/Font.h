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

class FontManager;

class Font
{
	DISABLE_COPY_AND_ASSIGN(Font);
	HANDLER_KLASS_NORMAL;
public:
	enum Flag {
		Italic = 0,
		Bold = 1
	};
private:
	FontManager& parent_;
	FT_Face face_;
	DEFINE_MEMBER(private, private, bool, locked);
private:
	Font(FontManager& parent, FT_Face face);
	friend class FontManager;
public:
	~Font();
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
};

class FontManager {
	friend class Font;
	DISABLE_COPY_AND_ASSIGN(FontManager);
	HANDLER_KLASS_NORMAL;
	DEFINE_MEMBER_REF(private, logging::Logger, log);
	DEFINE_MEMBER_CONST(protected, std::string, fontdir);
private:
	static constexpr std::size_t MaxUnusedFonts = 100;
	FT_Library freetype_;
	std::deque<Font*> unusedFonts_;
	Handler<Font> defaultFont_;
public:
	FontManager(logging::Logger& log, const std::string& fontdir);
	~FontManager() noexcept;
public:
	Handler<Font> queryFont(const std::string& name = std::string());
private:
	void backFont(Font* font);
private:
	Font* seachFont( const std::string& name );
	Font* seachDefaultFont();
};

}}
