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

#include <tarte/XVal.h>
#include "../Widget.h"
#include "../../gl/Sprite.h"

namespace chisa {
namespace util{
class ParamSet;
}

namespace tk {

class ImageWidget: public chisa::tk::Widget {
	DISABLE_COPY_AND_ASSIGN(ImageWidget);
private:
	Handler<gl::Sprite> imageSprite_;
	Handler<XObject> conf_;
public:
	ImageWidget(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* element);
	virtual ~ImageWidget() noexcept = default;
public:
	virtual void renderImpl(gl::Canvas& cv, geom::Area const& area) override;
	virtual void idleImpl(const float delta_ms) override;
	virtual void reshapeImpl(geom::Box const& area) override;
	virtual geom::Box measureImpl(geom::Box const& constraint) override;
	virtual chisa::geom::Area findTargetImpl(std::string const& target);
};

}}
