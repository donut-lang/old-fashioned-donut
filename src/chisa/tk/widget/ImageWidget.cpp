/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "ImageWidget.hpp"
#include "../World.hpp"
#include "../../gl/DrawableManager.hpp"

namespace chisa {
namespace tk {

const static std::string TAG("ImageWidget");

ImageWidget::ImageWidget(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element)
:Widget(log, world, element)
,conf_(new XObject( element->FirstChildElement() ))
{
	if( Handler<World> w = world.lock() ) {
		if(this->conf_->has<XString>("src")){
			std::string src = this->conf_->get<XString>("src");
			std::string fpath = w->resolveUniverseFilepath(src);
			this->imageSprite_ = w->drawableManager()->queryImage(fpath);
			if(this->imageSprite_ && log.t()){
				log.t(TAG, "file: %s loaded: %dx%d", fpath.c_str(), imageSprite_->width(), imageSprite_->height());
			}else if(!this->imageSprite_){
				log.e(TAG, "Failed to load: %s", fpath.c_str());
			}
		}else{
			log.e(TAG, "Oops. Src parameter not found!");
		}
	}else{
		log.e(TAG, "Oops. failed to lock world!");
	}
}

void ImageWidget::renderImpl(gl::Canvas& cv, geom::Area const& area)
{
	if(this->imageSprite_){
		cv.drawSprite(this->imageSprite_, geom::ZERO, area);
	}
}

void ImageWidget::idleImpl(const float delta_ms)
{
}

void ImageWidget::reshapeImpl(geom::Box const& area)
{
}

geom::Box ImageWidget::measureImpl(geom::Box const& constraint)
{
	if(imageSprite_){
		return geom::Box(imageSprite_->width(), imageSprite_->height());
	}else{
		log().e(TAG, "Image Sprite not allocated, but size measured.");
		return geom::Box(0, 0);
	}
}

chisa::geom::Area ImageWidget::findTargetImpl(const std::string& target)
{
	int x;
	int y;
	int w = 0;
	int h = 0;
	if(sscanf(target.c_str(), "%dx%dx%dx%d", &x, &y, &w, &h) < 2) {
		return chisa::geom::Area();
	}
	return chisa::geom::Area(x,y,w,h);
}
}}
