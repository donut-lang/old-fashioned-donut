/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <deque>
#include <cinamo/ClassUtil.h>
#include "../ImageFormat.hpp"

namespace chisa {
using namespace cinamo;

namespace gl {
class Sprite;

namespace internal {
class Buffer;

class SpriteManager: public HandlerBody<SpriteManager> {
	DEFINE_MEMBER_REF(private, Logger, log)
	Canvas& canvas_;
private:
	int maxTextureSize_;
private:
	static constexpr size_t MaxCachedSpriteCount = 200;
	static constexpr size_t MaxCachedBufferCount = 200;
	std::deque<Sprite*> unusedSprite_;
	std::deque<internal::Buffer*> unusedBuffer_;
public:
	SpriteManager(Logger& log, Canvas& canvas);
	virtual ~SpriteManager() noexcept;
	inline bool onFree() noexcept { return false; };
	Handler<Sprite> queryRawSprite(ImageFormat const foramt, const int width, const int height);
	inline int maxTextureSize() const noexcept { return this->maxTextureSize_; };
public:
	void backSprite(Sprite* spr);
	internal::Buffer* queryBuffer(std::size_t size);
	void backBuffer(internal::Buffer* buffer);
};

}}}
