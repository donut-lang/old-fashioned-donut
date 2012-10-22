/*
 * ContentWidget.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: psi
 */

#include "ContentWidget.h"
#include "../../geom/Area.h"
#include "../../geom/Vector.h"
#include "Content/NodeReader.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace widget {

CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(ContentWidget)
{
	this->rootNode(NodeReader().parseTree(element));
}

ContentWidget::~ContentWidget()
{
}

void ContentWidget::render(gl::Canvas& cv, const geom::Area& area)
{
}

void ContentWidget::idle(const float delta_ms)
{
}

void ContentWidget::reshape(const geom::Box& areaSize)
{
}

geom::Box ContentWidget::measure(const geom::Box& constraintSize)
{
}

bool ContentWidget::onDownRaw(const float timeMs, const geom::Point& ptInWidget)
{
}

bool ContentWidget::onUpRaw(const float timeMs, const geom::Point& ptInWidget)
{
}

bool ContentWidget::onSingleTapUp(const float timeMs, const geom::Point& ptInWidget)
{
}

bool ContentWidget::onZoom(const float timeMs, const geom::Point& center, const float ratio)
{
}

}}}
