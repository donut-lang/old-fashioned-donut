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

#include "DrawableManager.h"

namespace chisa {
namespace gl {

DrawableManager::DrawableManager()
{
	// TODO Auto-generated constructor stub

}

Handler<Sprite> DrawableManager::queryRawSprite(const int width, const int height)
{
	return this->spriteManager_->queryRawSprite(width, height);
}

Handler<Sprite> DrawableManager::queryImage(const std::string& filename)
{
	return this->imageManager_->queryImage(filename);
}

Handler<Drawable> DrawableManager::queryDrawable(const std::string& repl, const geom::Box& box)
{
}

Handler<Font> DrawableManager::queryFont(const std::string& name)
{
	return this->fontManager_->queryFont(name);
}

Handler<TextDrawable> DrawableManager::queryText(const std::string str, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, const gl::Color& color, const gl::Color& backColor)
{

}

}}
