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

#ifndef _CXX_CHISA_TK_WIDGET_IMAGEWIDGET_H_
#define _CXX_CHISA_TK_WIDGET_IMAGEWIDGET_H_

#include "../Widget.h"
#include "../../gl/Sprite.h"

namespace chisa {
namespace util{
class ParamSet;
}

namespace tk {
namespace widget {

class ImageWidget: public chisa::tk::Widget {
	DISABLE_COPY_AND_ASSIGN(ImageWidget);
private:
	gl::Sprite::Handler imageSprite_;
	util::ParamSet* const paramSet_;
public:
	ImageWidget(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* element);
	virtual ~ImageWidget();
public:
	virtual void render(gl::Canvas& cv, const Area& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(const Box& area) override;
	virtual Box measure(const Box& constraint) override;
};

}}}
#endif /* INCLUDE_GUARD */
