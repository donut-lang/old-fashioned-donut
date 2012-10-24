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

#include "ContentWidget.h"
#include "ContentMeasurer.h"
#include "../../geom/Area.h"
#include "../../geom/Vector.h"
#include "Content/NodeReader.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace widget {

static std::string const TAG("ContentWidget");

CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(ContentWidget)
,lastWidth_(NAN)
{
	tinyxml2::XMLElement* docElem = element->FirstChildElement("doc");
	this->rootNode(NodeReader().parseTree(docElem));
	if(this->log().t()){
		this->log().t(TAG, "Content Parsed.");
	}
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
	this->measure(areaSize);
}

geom::Box ContentWidget::measure(const geom::Box& constraintSize)
{
	if(geom::isUnspecified(this->lastWidth()) || std::fabs(constraintSize.width()-this->lastWidth()) >= geom::VerySmall){
		this->lastWidth(constraintSize.width());
		this->lastSize(ContentMeasurer(constraintSize.width()).start(this->rootNode()));
	}
	return this->lastSize();
}

bool ContentWidget::onDownRaw(const float timeMs, const geom::Point& ptInWidget)
{
	return true;
}

bool ContentWidget::onUpRaw(const float timeMs, const geom::Point& ptInWidget)
{
	return true;
}

bool ContentWidget::onSingleTapUp(const float timeMs, const geom::Point& ptInWidget)
{
	return true;
}

bool ContentWidget::onZoom(const float timeMs, const geom::Point& center, const float ratio)
{
	return true;
}

}}}

