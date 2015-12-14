/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/ClassUtil.h>
#include <cinamo/Logger.h>
#include <cinamo/Handler.h>
#include <deque>
#include <vector>
#include "../../gl/Drawable.hpp"
#include "RenderObject.hpp"

namespace chisa {
namespace gl {
class Drawable;
class Sprite;
class Canvas;
}

namespace doc {
class RenderObject;

class RenderTree : public HandlerBody<RenderTree> {
	DEFINE_MEMBER_REF(private, Logger, log);
private:
	const std::size_t maxDrawable_;
	Handler<gl::DrawableManager> drawableManager_;
	std::deque<Handler<gl::Drawable> > drawableCache_;
	std::vector<Handler<RenderObject> > objects_;
public:
	RenderTree(Logger& log, Handler<gl::DrawableManager> drawableManager, const std::size_t maxDrawable=100);
	virtual ~RenderTree() noexcept = default;
public:
	void render(gl::Canvas& canvas, geom::Area const& area, float depth);
	Handler<gl::DrawableManager> drawableManager();
public:
	void registerDrawable(Handler<gl::Drawable> d) noexcept;
	void reset() noexcept;
	bool onFree();
public:
	void addObject(Handler<RenderObject> obj);
};

}}
