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

#include "../../Widget.h"
#include "../Content/Node.h"
#include "../Content/NodeWalker.h"
#include "../../../geom/Area.h"
#include "../../../gl/StringRenderer.h"
#include "../../../gl/Sprite.h"

namespace chisa {
namespace gl {
class Drawable;
}

namespace tk {
namespace widget {
class RenderCache;

class RenderCommand : public HandlerBody<RenderCommand> {
	DISABLE_COPY_AND_ASSIGN(RenderCommand);
	HandlerW<RenderCache> cache_;
	geom::Area area_;
protected:
	RenderCommand() noexcept = delete;
	RenderCommand(HandlerW<RenderCache> cache, const geom::Area& area) noexcept:cache_(cache), area_(area){};
	HandlerW<RenderCache> cache() { return this->cache_; };
public:
	virtual ~RenderCommand() noexcept (true) = default;
	virtual std::string toString() const = 0;
	virtual void render(gl::Canvas& canvas, const geom::Point& offset, float depth=0.0f) = 0;
	virtual void onHidden() noexcept {};
public:
	virtual void onMoved(const geom::Area& before, const geom::Area& now) {};
	inline geom::Area& area() noexcept { return this->area_; };
	inline void move(const geom::Area& area) { this->onMoved(this->area_, area); this->area_=area; };
public:
	void onFree() noexcept { delete this; };
};

class SpriteRenderCommand : public RenderCommand {
	DEFINE_MEMBER(public, private, Handler<gl::Sprite>, sprite);
	DEFINE_MEMBER(public, private, HandlerW<gl::Sprite>, spritew);
public:
	SpriteRenderCommand(HandlerW<RenderCache> cache, const geom::Area& area) noexcept:RenderCommand(cache, area){};
	virtual ~SpriteRenderCommand() noexcept {};
		virtual std::string toString() const override;
public:
	Handler<gl::Sprite> realize(gl::Canvas& cv);
	virtual void onHidden() noexcept override;
	virtual void render(gl::Canvas& canvas, const geom::Point& offset, float depth=0.0f) override;
protected:
	virtual Handler<gl::Sprite> realizeImpl(gl::Canvas& cv) = 0;
	void invalidate() noexcept;
};

class TextRenderCommand : public SpriteRenderCommand {
private:
	gl::StringRenderer::Command cmd_;
public:
	TextRenderCommand(HandlerW<RenderCache> cache, const geom::Area& area, const gl::StringRenderer::Command& cmd) noexcept
	:SpriteRenderCommand(cache, area),cmd_(cmd){};
	virtual ~TextRenderCommand() noexcept = default;
		virtual std::string toString() const override;
protected:
	virtual Handler<gl::Sprite> realizeImpl(gl::Canvas& cv) override;
};

class DrawableRenderCommand : public RenderCommand {
private:
	Handler<gl::Drawable> drawable_;
	HandlerW<gl::Drawable> drawablew_;
	std::string drawableRepl_;
public:
	DrawableRenderCommand(HandlerW<RenderCache> cache, const geom::Area& area, const std::string& drawableRepl);
	virtual ~DrawableRenderCommand() noexcept = default;
		virtual std::string toString() const override;
public:
	virtual void onHidden() noexcept override;
	virtual void render(gl::Canvas& canvas, const geom::Point& offset, float depth=0.0f) override;
private:
	void realize(gl::Canvas& canvas);
};

}}}
