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
#include "../../geom/Area.h"
#include "../../geom/Vector.h"
#include "Content/NodeReader.h"
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace widget {

static std::string const TAG("ContentWidget");

CHISA_WIDGET_SUBKLASS_CONSTRUCTOR_DEF(ContentWidget)
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

