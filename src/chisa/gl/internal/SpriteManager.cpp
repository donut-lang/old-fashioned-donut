/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include "../Sprite.h"
#include "SpriteManager.h"
#include "Buffer.h"
#include "Order.h"

namespace chisa {
namespace gl{
namespace internal {
static const std::string TAG("RawSpriteManager");

SpriteManager::SpriteManager(Logger& log, Canvas& canvas)
:log_(log)
,canvas_(canvas)
,maxTextureSize_(0)
{
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize_ );
	log_.i(TAG, "Max texture size:%dx%d ", maxTextureSize_, maxTextureSize_);
	maxTextureSize_ = std::max(1024, maxTextureSize_);
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

Handler<Sprite> SpriteManager::queryRawSprite(ImageFormat const format, const int width, const int height)
{
	typedef chisa::gl::internal::WidthOrder<Sprite> SpriteOrder;
	auto it = std::lower_bound(this->unusedSprite_.begin(), this->unusedSprite_.end(),
			std::tuple<ImageFormat, int,int>(format, width, height), Sprite::CompareByTexture());
	//横幅が同じ場合は、縦幅も大きいか同じであることが保証される。
	//横幅が優先なので、横幅が違う場合は高さは短いかもしれない
	if(it == this->unusedSprite_.end() || (*it)->texture().format() != format || (*it)->texture().height() < height || (*it)->texture().width() < width) {
		Handler<Sprite> spr ( new Sprite(this, format, geom::IntBox(width, height)) );
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

}}}
