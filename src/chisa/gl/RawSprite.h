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
#include <cairo/cairo.h>
#include "../logging/Logger.h"
#include "../util/ClassUtil.h"
#include "../util/Thread.h"
#include "../Handler.h"
#include "Buffer.h"
#include "Sprite.h"
#include <deque>

namespace chisa {
namespace gl {

class RawSpriteManager {
	DISABLE_COPY_AND_ASSIGN(RawSpriteManager);
	DEFINE_MEMBER_REF(private, logging::Logger, log)
private:
	static constexpr size_t MaxCachedSpriteCount = 200;
	static constexpr size_t MaxCachedBufferCount = 200;
	std::deque<RawSprite*> unusedSprite_;
	std::deque<Buffer*> unusedBuffer_;
public:
	RawSpriteManager(logging::Logger& log);
	~RawSpriteManager() noexcept;
	Handler<RawSprite> queryRawSprite(const int width, const int height);
public:
	void backSprite(RawSprite* spr);
	Buffer* queryBuffer(const int width, const int height);
	void backBuffer(Buffer* buffer);
};

class RawSprite : public Sprite {
	RawSpriteManager* const mgr_;
	DEFINE_MEMBER(public, private, int, origWidth);
	DEFINE_MEMBER(public, private, int, origHeight);
	DEFINE_MEMBER(public, private, int, width);
	DEFINE_MEMBER(public, private, int, height);
	unsigned int texId_;
	std::atomic<bool> locked_;
public:
	RawSprite(RawSpriteManager* const mgr, const int width, const int height);
	virtual ~RawSprite() noexcept(true);
private:
	Buffer* lock();
	void unlock();
private:
	virtual void onFree() noexcept override;
	virtual void drawImpl(Canvas* const canvas, const geom::Point& pt, const float depth) override;
	Buffer* buffer_;
public:
	class Session {
		DISABLE_COPY_AND_ASSIGN(Session);
		STACK_OBJECT(Session);
	private:
		Handler<RawSprite> parent_;
	public:
		Session(Handler<RawSprite> parent);
		~Session();
		inline int width() const { return parent_->width(); };
		inline int height() const { return parent_->height(); };
		inline int stride() const { return parent_->buffer_->stride(); };
		inline unsigned char* data() const { return parent_->buffer_->data(); };
	};
public:
	void resize(int width, int height);
};

}}
