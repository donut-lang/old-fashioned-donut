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
#include <memory>
#include <functional>
#include <tinyxml2.h>
#include <vector>
#include "../../logging/Logger.h"
#include "../../util/ClassUtil.h"
#include "../../util/VectorMap.h"

namespace chisa {
namespace util {
class ParamSet;
class Param;
}
namespace tk {
class World;
class Widget;
namespace widget {

template <typename WidgetKlass>
WidgetKlass* widgetConstructor(logging::Logger& log, std::weak_ptr<World> world, tinyxml2::XMLElement* elem){
	return new WidgetKlass(log, world, elem);
}

class WidgetFactory {
	DISABLE_COPY_AND_ASSIGN(WidgetFactory);
private:
	logging::Logger& log_;
	std::weak_ptr<World> world_;
	typedef std::function<Widget*(logging::Logger& log, std::weak_ptr<World> world, tinyxml2::XMLElement* elem)> ConstructorType;
	util::VectorMap<std::string, ConstructorType> widgetMap_;
public:
	WidgetFactory(logging::Logger& log, std::weak_ptr<World> world);
	virtual ~WidgetFactory();
public:
	void registerWidget(const std::string& klass, std::function<Widget*(logging::Logger& log, std::weak_ptr<World> world, tinyxml2::XMLElement* elem)> func);
	template <typename WidgetKlass>
	void registerWidget(const std::string& klass) {
		this->registerWidget(klass, widgetConstructor<WidgetKlass>);
	}

	Widget* createWidget(const std::string& klass, tinyxml2::XMLElement* elem);
};

}}}
