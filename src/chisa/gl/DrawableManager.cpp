/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "DrawableManager.h"

namespace chisa {
namespace gl {

DrawableManager::DrawableManager(Logger& log, Canvas& canvas, DrawableSetting const& setting)
:log_(log)
,canvas_(canvas)
,spriteManager_(new gl::internal::SpriteManager(log, canvas))
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

int DrawableManager::maxTextureSize() const noexcept
{
	return this->spriteManager_->maxTextureSize();
}

Handler<Sprite> DrawableManager::queryImage(std::string const& filename)
{
	return this->imageManager_->queryImage(filename);
}

Handler<Drawable> DrawableManager::queryDrawable(std::string const& repl, geom::Box const& box)
{
	for(std::pair<std::string, constructor> p : this->factories_){
		if(::cinamo::startsWith(repl, p.first)){
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
