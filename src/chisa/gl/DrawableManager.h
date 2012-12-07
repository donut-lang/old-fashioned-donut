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
#include <deque>
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"
#include "../util/VectorMap.h"
#include "Sprite.h"
#include "Font.h"
#include "internal/ImageManager.h"
#include "Drawable.h"
#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

#ifdef Handler
#undef Handler
#endif

namespace chisa {
namespace gl {

class DrawableSetting {
	DEFINE_MEMBER(public, public, std::string, fontdir);
public:
	DrawableSetting( const DrawableSetting& other) = default;
	DrawableSetting( DrawableSetting&& other) = default;
	DrawableSetting& operator=( const DrawableSetting& other) = default;
	DrawableSetting& operator=( DrawableSetting&& other) = default;
	~DrawableSetting() noexcept = default;
	DrawableSetting( const std::string& fontdir ):fontdir_(fontdir){};
};

//---------------------------------------------------------------------------------------------------------------------

class DrawableManager : public HandlerBody<DrawableManager> {
	DEFINE_MEMBER_REF(private, logging::Logger, log);
private:
	typedef std::function<Handler<Drawable>(HandlerW<DrawableManager>, const geom::Box&, const std::string&)> constructor;
	util::VectorMap<std::string, constructor> factories_;
private:
	Handler<internal::SpriteManager> spriteManager_;
	Handler<internal::ImageManager> imageManager_;
	Handler<internal::FontManager> fontManager_;
public:
	DrawableManager(logging::Logger& log, const DrawableSetting& setting);
	virtual ~DrawableManager() noexcept = default;
public:
	Handler<Sprite> queryRawSprite(const int width, const int height);
	Handler<Sprite> queryImage(const std::string& filename);
	Handler<Drawable> queryDrawable(const std::string& repl, const geom::Box& box=geom::Box());
	Handler<Font> queryFont( const std::string& name = std::string() );
	Handler<TextDrawable> queryText(
			const std::string& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			const gl::Color& color=gl::Black,
			const gl::Color& backColor=gl::Transparent
		);
	Handler<TextDrawable> queryVerticalText(
			const std::string& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			const gl::Color& color=gl::Black,
			const gl::Color& backColor=gl::Transparent
		);
public:
	bool onFree() noexcept { return false; };
};

}}
