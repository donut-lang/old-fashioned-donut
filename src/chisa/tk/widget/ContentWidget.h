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

#include <tarte/Handler.h>

#include "../Widget.h"
#include "../../geom/Vector.h"
#include "../../gl/Font.h"
#include "../../doc/render/RenderTree.h"

namespace chisa {
namespace doc {
class Document;
}

namespace tk {

class ContentWidget: public chisa::tk::Widget {
	CHISA_WIDGET_SUBKLASS(ContentWidget);
	DEFINE_MEMBER(private, private, std::shared_ptr<doc::Document>, rootNode);
	Handler<doc::RenderTree> renderTree_;
	DEFINE_MEMBER(private, private, float, lastWidth);
	DEFINE_MEMBER(private, private, geom::Box, lastSize);
public:
	virtual void render(gl::Canvas& cv, geom::Area const& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(geom::Box const& areaSize) override;
	virtual geom::Box measure(geom::Box const& constraintSize) override;
public:
	virtual bool onDownRaw(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onUpRaw(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onSingleTapUp(float const& timeMs, geom::Point const& ptInWidget) override;
	virtual bool onZoom(float const& timeMs, geom::Point const& center, const float ratio) override;
};

}}
