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
#include "../Handler.h"
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"
#include "internal/FontManager.h"

namespace chisa {
namespace gl {

class Font : public HandlerBody<Font>
{
public:
	enum Flag {
		Italic = 0,
		Bold = 1
	};
private:
	HandlerW<internal::FontManager> parent_;
	Handler<internal::FreeType> freetype_;
	FT_Face face_;
	DEFINE_MEMBER(private, private, bool, locked);
public:
	Font(internal::FontManager* parent, Handler<internal::FreeType> freetype, FT_Face face);
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
	bool onFree();
};

}}
