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

#include "ImageWidget.h"
#include "../World.h"
#include "../../util/Param.h"

namespace chisa {
namespace tk {
namespace widget {

const static std::string TAG("ImageWidget");

ImageWidget::ImageWidget(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* element)
:Widget(log, world, element)
,paramSet_(new util::ParamSet())
{
	paramSet_->parseTree(element);
	if(shared_ptr<World> w = world.lock()) {
		if(paramSet_->has("src")){
			std::string src;
			if(paramSet_->get("src")->queryString(&src)){
				std::string fpath = w->resolveUniverseFilepath(src);
				this->imageSprite_ = w->queryImage(fpath);
				if(this->imageSprite_ && log.t()){
					log.t(TAG, "file: %s loaded: %dx%d", fpath.c_str(), imageSprite_->width(), imageSprite_->height());
				}else if(!this->imageSprite_){
					log.e(TAG, "Failed to load: %s", fpath.c_str());
				}
			}else{
				log.e(TAG, "Oops. Src parameter is not string.");
			}
		}else{
			log.e(TAG, "Oops. Src parameter not found!");
		}
	}else{
		log.e(TAG, "Oops. failed to lock world!");
	}
}

ImageWidget::~ImageWidget()
{
	delete paramSet_;
}

void ImageWidget::render(gl::Canvas& cv, const geom::Area& area)
{
	if(this->imageSprite_){
		cv.drawSprite(this->imageSprite_, geom::ZERO, &area);
	}
}

void ImageWidget::idle(const float delta_ms)
{
}

void ImageWidget::reshape(const geom::Box& area)
{
}

geom::Box ImageWidget::measure(const geom::Box& constraint)
{
	if(imageSprite_){
		return geom::Box(imageSprite_->width(), imageSprite_->height());
	}else{
		log().e(TAG, "Image Sprite not allocated, but size measured.");
		return geom::Box(0, 0);
	}
}

}}}
