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

#include "RenderCache.h"
#include "../../../gl/Drawable.h"
#include "../../../gl/Sprite.h"
namespace chisa {
namespace tk {
namespace widget {

RenderCache::RenderCache(const std::size_t maxSprites,const std::size_t maxDrawable)
:maxSprites_(maxSprites), maxDrawable_(maxDrawable)
{
}

void RenderCache::registerDrawable(Handler<gl::Drawable> d) noexcept
{
	this->drawableCache_.push_back(d);
	while(this->drawableCache_.size() > this->maxSprites_){
		this->drawableCache_.pop_front();
	}
}

void RenderCache::registerSprite(Handler<gl::Sprite> s) noexcept
{
	this->spriteCache_.push_back(s);
	while (this->spriteCache_.size() > this->maxSprites_) {
		this->spriteCache_.pop_front();
	}
}

bool RenderCache::unregisterDrawable(const Handler<gl::Drawable>& d) noexcept
{
	for(auto it = this->drawableCache_.begin(); it != this->drawableCache_.end(); ++it){
		if(d == *it){
			this->drawableCache_.erase(it);
			return true;
		}
	}
	return false;
}

bool RenderCache::unregisterSprite(const Handler<gl::Sprite>& s) noexcept
{
	for(auto it = this->spriteCache_.begin(); it != this->spriteCache_.end(); ++it){
		if(s == *it){
			this->spriteCache_.erase(it);
			return true;
		}
	}
	return false;
}

void RenderCache::reset() noexcept
{
	(decltype(this->spriteCache_)()).swap(this->spriteCache_);
	(decltype(this->drawableCache_)()).swap(this->drawableCache_);
}

void RenderCache::onFree()
{
	delete this;
}

}}}
