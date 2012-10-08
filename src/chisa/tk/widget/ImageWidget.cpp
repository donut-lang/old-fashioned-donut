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
			}
		}
	}
}

ImageWidget::~ImageWidget()
{
	delete paramSet_;
}

void ImageWidget::render(gl::Canvas& cv, const Area& area)
{
	if(this->imageSprite_){
		cv.drawSprite(-area.x(), -area.y(), 0, this->imageSprite_);
	}
}

void ImageWidget::idle(const float delta_ms)
{
}

void ImageWidget::reshape(const Box& area)
{
}

Box ImageWidget::measure(const Box& constraint)
{
	if(imageSprite_){
		return Box(imageSprite_->width(), imageSprite_->height());
	}else{
		return Box(0, 0);
	}
}

}}}
