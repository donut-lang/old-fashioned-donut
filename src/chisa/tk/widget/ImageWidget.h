/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

#include <cinamo/XVal.h>
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
