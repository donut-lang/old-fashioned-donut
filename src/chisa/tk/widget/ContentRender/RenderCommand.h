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

namespace chisa {
namespace tk {
namespace widget {

class RenderCommand : public HandlerBody<RenderCommand> {
	DISABLE_COPY_AND_ASSIGN(RenderCommand);
	DEFINE_MEMBER(public, private, geom::Area, area);
protected:
	RenderCommand() noexcept = delete;
	RenderCommand(const geom::Area& area) noexcept:area_(area){};
public:
	virtual ~RenderCommand() noexcept (true) = default;
	virtual void execute(gl::Canvas& canvas, const geom::Point& offset) = 0;
	virtual void free() noexcept {};
};

class SpriteRenderCommand : public RenderCommand {
	DEFINE_MEMBER(public, private, Handler<gl::Sprite>, sprite);
public:
	SpriteRenderCommand(const geom::Area& area) noexcept:RenderCommand(area){};
	virtual ~SpriteRenderCommand() noexcept {};
public:
	Handler<gl::Sprite> realize(gl::Canvas& cv);
	virtual void free() noexcept override;
	virtual void execute(gl::Canvas& canvas, const geom::Point& offset) override;
protected:
	virtual Handler<gl::Sprite> realizeImpl(gl::Canvas& cv) = 0;
};

class TextRenderCommand : public SpriteRenderCommand {
private:
	gl::StringRenderer::Command cmd_;
public:
	TextRenderCommand(const geom::Area& area, const gl::StringRenderer::Command& cmd) noexcept
	:SpriteRenderCommand(area),cmd_(cmd){};
	virtual ~TextRenderCommand() noexcept = default;
protected:
	virtual Handler<gl::Sprite> realizeImpl(gl::Canvas& cv) override;
};

class UnderlineRenderCommand : public RenderCommand {
private:
	gl::Color color_;
	float width_;
public:
	UnderlineRenderCommand(const geom::Area& area, const gl::Color& color, const float width) noexcept
	:RenderCommand(area), color_(color), width_(width){};
	virtual ~UnderlineRenderCommand() noexcept (true) = default;
public:
	virtual void execute(gl::Canvas& canvas, const geom::Point& offset) override;
};

class BackgroundRenderCommand : public RenderCommand {
public:
	enum Type {
		Color,
		Image
	};
};

}}}
