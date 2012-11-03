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
#include "../logging/Logger.h"
#include "Sprite.h"
#include "Font.h"
#include "internal/ImageManager.h"
#include "Drawable.h"

namespace chisa {
namespace gl {

class DrawableManager : public HandlerBody<DrawableManager> {
private:
	Handler<internal::SpriteManager> spriteManager_;
	Handler<internal::ImageManager> imageManager_;
	Handler<internal::FontManager> fontManager_;
public:
	DrawableManager();
	virtual ~DrawableManager() noexcept = default;
public:
	Handler<Sprite> queryRawSprite(const int width, const int height);
	Handler<Sprite> queryImage(const std::string& filename);
	Handler<Drawable> queryDrawable(const std::string& repl, const geom::Box& box=geom::Box());
	Handler<Font> queryFont( const std::string& name );
	Handler<TextDrawable> queryText(
			const std::string str,
			const float size=TextDrawable::DefaultFontSize,
			Handler<Font> font=Handler<Font>(),
			TextDrawable::Style style=TextDrawable::Style::Regular,
			TextDrawable::Decoration deco=TextDrawable::Decoration::None,
			const gl::Color& color=BLACK,
			const gl::Color& backColor=TRANSPARENT
		);
public:
	void onFree() noexcept { delete this; };
};

}}
