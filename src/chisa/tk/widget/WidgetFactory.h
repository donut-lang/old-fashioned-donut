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

#ifndef __CXX_CHISA_TK_WIDGET_WIDGETFACTORY_H_
#define __CXX_CHISA_TK_WIDGET_WIDGETFACTORY_H_

#include <memory>
#include <functional>
#include <tinyxml2.h>
#include "../../logging/Logger.h"
#include "../../util/class_utils.h"

namespace chisa {
namespace util {
class ParamSet;
class Param;
}
namespace tk {
class World;
class Widget;
namespace widget {

using namespace std;

class WidgetFactory {
	DISABLE_COPY_AND_ASSIGN(WidgetFactory);
public:
	template <typename WidgetKlass>
	WidgetKlass* widgetConstructor(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* elem){
		return new WidgetKlass(log, world, elem);
	}
private:
	logging::Logger& log_;
	weak_ptr<World> world_;
public:
	WidgetFactory(logging::Logger& log, weak_ptr<World> world);
	virtual ~WidgetFactory();
public:
	void registerWidget(const string& klass, std::function<Widget*(logging::Logger& log, weak_ptr<World> world, tinyxml2::XMLElement* elem)> func);
	template <typename WidgetKlass>
	void registerWidget(const string& klass) {
		this->registerWidget(klass, widgetConstructor<WidgetKlass>);
	}

	Widget* createWidget(const string& klass, tinyxml2::XMLElement* elem);
};

}}}
#endif /* INCLUDE_GURAD */
