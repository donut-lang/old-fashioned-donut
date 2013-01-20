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
#include <tarte/ClassUtil.h>
#include <tarte/Logger.h>
#include <tarte/VectorMap.h>
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
using namespace tarte;

namespace gl {

class DrawableSetting {
	DEFINE_MEMBER(public, public, std::string, fontdir);
public:
	DrawableSetting( DrawableSetting const& other) = default;
	DrawableSetting( DrawableSetting&& other) = default;
	DrawableSetting& operator=( DrawableSetting const& other) = default;
	DrawableSetting& operator=( DrawableSetting&& other) = default;
	~DrawableSetting() noexcept = default;
	DrawableSetting( std::string const& fontdir ):fontdir_(fontdir){};
};

//---------------------------------------------------------------------------------------------------------------------

class DrawableManager : public HandlerBody<DrawableManager> {
	DEFINE_MEMBER_REF(private, Logger, log);
	Canvas& canvas_;
private:
	typedef std::function<Handler<Drawable>(HandlerW<DrawableManager>, geom::Box const&, std::string const&)> constructor;
	VectorMap<std::string, constructor> factories_;
private:
	Handler<internal::SpriteManager> spriteManager_;
	Handler<internal::ImageManager> imageManager_;
	Handler<internal::FontManager> fontManager_;
public:
	DrawableManager(Logger& log, Canvas& canvas, DrawableSetting const& setting);
	virtual ~DrawableManager() noexcept = default;
public:
	Handler<Sprite> queryRawSprite(ImageFormat const format, const int width, const int height);
	Handler<Sprite> queryImage(std::string const& filename);
	Handler<Drawable> queryDrawable(std::string const& repl, geom::Box const& box=geom::Box());
	Handler<Font> queryFont( std::string const& name = std::string() );
	Handler<TextDrawable> queryText(
			std::string const& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			gl::Color const& color=gl::Black,
			gl::Color const& backColor=gl::Transparent
		);
	Handler<TextDrawable> queryVerticalText(
			std::string const& str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			gl::Color const& color=gl::Black,
			gl::Color const& backColor=gl::Transparent
		);
public:
	bool onFree() noexcept { return false; };
};

}}
