/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include <cinamo/ClassUtil.h>
#include "../../geom/Area.hpp"
#include "../../gl/Drawable.hpp"

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
	geom::Area area_;
	DEFINE_MEMBER(public, public, float, relDepth);
public:
	RenderObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth);
	virtual ~RenderObject() noexcept = default;
	inline geom::Area const& area() const noexcept { return this->area_; };
	inline geom::Area& area() noexcept { return this->area_; };
	inline void area(geom::Area const& narea) noexcept { this->area_=narea; };
public:
	bool onFree() noexcept { return false; };
	void render(gl::Canvas& canvas, const float depth);
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
	NormalDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, std::string const& repl, geom::Box const& size);
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
	TextDrawableObject(HandlerW<RenderTree> parentTree, Node* parentNode, const float relDepth, std::string const& str, const float size, Handler<gl::Font> font, gl::TextDrawable::Style style, gl::TextDrawable::Decoration deco, gl::Color const& color, gl::Color const& backColor);
	virtual ~TextDrawableObject() noexcept = default;
	virtual Handler<gl::Drawable> realize() override;
};

}}
