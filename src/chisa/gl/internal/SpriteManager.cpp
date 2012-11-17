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

#include "../Sprite.h"
#include "Order.h"
#include <algorithm>

namespace chisa {
namespace gl{
namespace internal {
static const std::string TAG("RawSpriteManager");

SpriteManager::SpriteManager(logging::Logger& log)
:log_(log)
{

}
SpriteManager::~SpriteManager() noexcept
{
	for(internal::Buffer* b : this->unusedBuffer_){
		delete b;
	}
	for(Sprite* s : this->unusedSprite_){
		delete s;
	}
	decltype(this->unusedBuffer_)().swap(this->unusedBuffer_);
	decltype(this->unusedSprite_)().swap(this->unusedSprite_);
}

void SpriteManager::backSprite(Sprite* spr)
{
	typedef chisa::gl::internal::WidthOrder<Sprite> SpriteOrder;
	auto ins = std::upper_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), spr, SpriteOrder());
	this->unusedSprite_.insert(ins, spr);
	while(MaxCachedSpriteCount < this->unusedSprite_.size()){
		Sprite* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedSprite_.back();
			this->unusedSprite_.pop_back();
		}else{
			deleted = this->unusedSprite_.front();
			this->unusedSprite_.	pop_front();
		}
		if(deleted){
			Sprite* const min = this->unusedSprite_.front();
			Sprite* const max = this->unusedSprite_.back();
			if(this->log().d()){
				this->log().d(TAG, "Sprite cache deleted. size: %dx%d / min:%dx%d, max:%dx%d",
						deleted->width(), deleted->height(),
						min->width(), min->height(),
						max->width(), max->height());
			}
			delete deleted;
		}
	}
}

Handler<Sprite> SpriteManager::queryRawSprite(const int width, const int height)
{
	typedef chisa::gl::internal::WidthOrder<Sprite> SpriteOrder;
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(), std::pair<int,int>(width,height), SpriteOrder());
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedSprite_.end() || (*it)->height() < height){
		Handler<Sprite> spr ( new Sprite(this, geom::IntBox(width, height)) );
		spr->resize(width, height);
		return spr;
	}else{
		Handler<Sprite> spr(*it);
		this->unusedSprite_.erase(it);
		spr->resize(width, height);
		return spr;
	}
}

internal::Buffer* SpriteManager::queryBuffer(const std::size_t size)
{
	typedef chisa::gl::internal::SizeOrder<internal::Buffer> BufferOrder;
	auto it = std::lower_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), size, BufferOrder());
	if(it == this->unusedBuffer_.end() || (*it)->size() >= size*2){
		return new internal::Buffer(size);
	}else{
		internal::Buffer* const buf = *it;
		this->unusedBuffer_.erase(it);
		return buf;
	}
}

void SpriteManager::backBuffer(internal::Buffer* buffer)
{
	typedef chisa::gl::internal::SizeOrder<internal::Buffer> BufferOrder;
	auto ins = std::upper_bound(this->unusedBuffer_.begin(), this->unusedBuffer_.end(), buffer, BufferOrder());
	this->unusedBuffer_.insert(ins, buffer);
	while(MaxCachedBufferCount < this->unusedBuffer_.size()){
		internal::Buffer* deleted = 0;
		if((rand() & 1U) == 1U){
			deleted = this->unusedBuffer_.back();
			this->unusedBuffer_.pop_back();
		}else{
			deleted = this->unusedBuffer_.front();
			this->unusedBuffer_.pop_front();
		}
		if(deleted){
			internal::Buffer* const min = this->unusedBuffer_.front();
			internal::Buffer* const max = this->unusedBuffer_.back();
			if(this->log().d()){
				this->log().d(TAG, "Buffer cache deleted. size: %d / min:%d, max:%d",
						deleted->size(), min->size(), max->size());
			}
			delete deleted;
		}
	}
}


//-----------------------------------------------------------------------------
Buffer::Buffer(std::size_t size)
:size_(size), ptr_(new unsigned char [size])
{

}
Buffer::~Buffer() noexcept
{
	delete [] ptr_;
}


}}}
