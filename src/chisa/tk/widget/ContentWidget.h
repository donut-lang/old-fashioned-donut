/*
 * ContentWidget.h
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#pragma once

#include "../Widget.h"
#include "Content/Decl.h"

namespace chisa {
namespace tk {
namespace widget {

class ContentWidget: public chisa::tk::Widget {
	CHISA_WIDGET_SUBKLASS(ContentWidget);
	DEFINE_MEMBER(private, private, std::shared_ptr<Model>, rootModel);
public:
	virtual void render(gl::Canvas& cv, const geom::Area& area) override;
	virtual void idle(const float delta_ms) override;
	virtual void reshape(const geom::Box& areaSize) override;
	virtual geom::Box measure(const geom::Box& constraintSize) override;
public:
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInWidget) override;
	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio) override;
};

}}}
