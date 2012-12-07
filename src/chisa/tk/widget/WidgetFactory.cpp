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

#include <algorithm>

#include "WidgetFactory.h"
#include "ImageWidget.h"
#include "ContentWidget.h"

namespace chisa {
namespace tk {
namespace widget {

WidgetFactory::WidgetFactory(logging::Logger& log, std::weak_ptr<World> world)
:log_(log)
,world_(world)
{
	this->registerWidget<ImageWidget>("image");
	this->registerWidget<ContentWidget>("content");
}

WidgetFactory::~WidgetFactory()
{
}

void WidgetFactory::registerWidget(const std::string& klass, std::function<Widget*(logging::Logger& log, std::weak_ptr<World> world, tinyxml2::XMLElement* elem)> func)
{
	auto it = std::lower_bound(this->widgetMap_.begin(), this->widgetMap_.end(), klass, Comparator());
	std::pair<std::string, ConstructorType>& p = *it;
	if( it == this->widgetMap_.end() || p.first != klass ){
		this->widgetMap_.insert( it, std::make_pair(klass, func) );
	}else{
		p.second = func;
	}
}

Widget* WidgetFactory::createWidget(const std::string& klass, tinyxml2::XMLElement* elem)
{
	auto it = std::lower_bound(this->widgetMap_.begin(), this->widgetMap_.end(), klass, Comparator());
	std::pair<std::string, ConstructorType>& p = *it;
	if(it == this->widgetMap_.end() || p.first != klass){
		return nullptr;
	}
	return p.second(log_, world_, elem);
}

}}}
