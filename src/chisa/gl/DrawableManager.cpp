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

DrawableManager::DrawableManager(Logger& log, DrawableSetting const& setting)
:log_(log)
,spriteManager_(new gl::internal::SpriteManager(log))
,imageManager_(new gl::internal::ImageManager(log, this->spriteManager_))
,fontManager_(new gl::internal::FontManager(log, setting.fontdir()))
{
	this->factories_.insert("stretch:", StretchDrawable::create);
	this->factories_.insert("repeat:", RepeatDrawable::create);
	this->factories_.insert("image:", ImageDrawable::create);
	this->factories_.insert("color:", ColorDrawable::create);
	this->factories_.insert("none:", NullDrawable::create);
}

Handler<Sprite> DrawableManager::queryRawSprite(ImageFormat const format, const int width, const int height)
{
	return this->spriteManager_->queryRawSprite(format, width, height);
}

Handler<Sprite> DrawableManager::queryImage(std::string const& filename)
{
	return this->imageManager_->queryImage(filename);
}

Handler<Drawable> DrawableManager::queryDrawable(std::string const& repl, geom::Box const& box)
{
	for(std::pair<std::string, constructor> p : this->factories_){
		if(::tarte::startsWith(repl, p.first)){
			std::string const left = repl.substr(p.first.size());
			return p.second(this->self(), box, left);
		}
	}
	this->log().w("DrawableFactory", "oops. Invalid repl: %s", repl.c_str());
	return NullDrawable::create(this->self(), box, repl);
}

Handler<Font> DrawableManager::queryFont(std::string const& name)
{
	return this->fontManager_->queryFont(name);
}

Handler<TextDrawable> DrawableManager::queryText(std::string const& str, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor)
{
	if(!font){
		font = this->fontManager_->queryFont();
	}
	return TextDrawable::create(this->self(), str, false, size, font, style, deco, color, backColor);
}

Handler<TextDrawable> DrawableManager::queryVerticalText(std::string const& str, const float size, Handler<Font> font, TextDrawable::Style style, TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor)
{
	if(!font){
		font = this->fontManager_->queryFont();
	}
	return TextDrawable::create(this->self(), str, true, size, font, style, deco, color, backColor);
}

}}
