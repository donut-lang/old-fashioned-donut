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
#include "../../Handler.h"
#include "../../util/ClassUtil.h"
#include "../../geom/Area.h"
#include "../../gl/Drawable.h"

namespace chisa {
namespace gl {
class Canvas;
}

namespace doc {
class RenderTree;
class Node;

class RenderObject : public HandlerBody<RenderObject> {
private:
	HandlerW<RenderTree> parentTree_;
	Node* parentNode_;
	bool dirty_;
	geom::Box cachedSize_;
	Handler<gl::Drawable> drawable_;
	HandlerW<gl::Drawable> drawablew_;
private:
	DEFINE_CONTAINER(public, public, geom::Area, area);
	DEFINE_MEMBER(public, public, float, relDepth);
public:
	RenderObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth);
	virtual ~RenderObject() noexcept = default;
public:
	void onFree() noexcept { delete this; };
	void render(gl::Canvas& canvas, const geom::Point& offset, const float depth);
	void onHidden();
	HandlerW<RenderTree> parentTree();
	Node* parentNode();
	geom::Box size();
	float width();
	float height();
private:
	Handler<gl::Drawable> drawable();
protected:
	virtual Handler<gl::Drawable> realize() = 0;
};

class NormalDrawableObject : public RenderObject {
private:
	std::string repl_;
	geom::Box size_;
public:
	NormalDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, const std::string& repl, const geom::Box& size);
	virtual ~NormalDrawableObject() noexcept = default;
	virtual Handler<gl::Drawable> realize() override;
};

class TextDrawableObject : public RenderObject {
private:
	std::string str_;
	float size_;
	Handler<gl::Font> font_;
	gl::TextDrawable::Style style_;
	gl::TextDrawable::Decoration deco_;
	gl::Color color_;
	gl::Color backColor_;
public:
	TextDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, const std::string& str, const float size, Handler<gl::Font> font, gl::TextDrawable::Style style, gl::TextDrawable::Decoration deco, const gl::Color& color, const gl::Color& backColor);
	virtual ~TextDrawableObject() noexcept = default;
	virtual Handler<gl::Drawable> realize() override;
};

}}
