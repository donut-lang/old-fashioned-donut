/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "RenderTree.hpp"
#include "../../gl/DrawableManager.hpp"

namespace chisa {
namespace doc {

const static std::string TAG("RenderTree");

RenderTree::RenderTree(Logger& log, Handler<gl::DrawableManager> drawableManager, const std::size_t maxDrawable)
:log_(log)
,maxDrawable_(maxDrawable)
,drawableManager_(drawableManager)
{
}

void RenderTree::render(gl::Canvas& canvas, geom::Area const& area, float depth)
{
	for(Handler<RenderObject>& obj : this->objects_){
		geom::Area const intersect(obj->area().intersect(area));
		if (!intersect.empty()) {
			obj->render(canvas, depth+obj->relDepth());
		}else{
			//obj->onHidden();
		}
	}
}

Handler<gl::DrawableManager> RenderTree::drawableManager()
{
	return this->drawableManager_;
}

void RenderTree::registerDrawable(Handler<gl::Drawable> d) noexcept
{
	this->drawableCache_.push_back(d);
	if(this->log().t()){
		this->log().t(TAG, "Drawable cache size: %d", this->drawableCache_.size());
	}
	while(this->drawableCache_.size() > this->maxDrawable_){
		if(this->log().t()){
			Handler<gl::Drawable> del = this->drawableCache_.front();
			this->log().t(TAG, "Drawable cache deleted: %s", del->toString().c_str());
		}
		this->drawableCache_.pop_front();
	}
}

void RenderTree::reset() noexcept
{
	(decltype(this->drawableCache_)()).swap(this->drawableCache_);
	(decltype(this->objects_)()).swap(this->objects_);
}

bool RenderTree::onFree()
{
	this->reset();
	return false;
}

void RenderTree::addObject(Handler<RenderObject> obj)
{
	this->objects_.push_back(obj);
}

}}
