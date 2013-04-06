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
#include <functional>
#include <tinyxml2.h>
#include <vector>
#include <cinamo/Handler.h>
#include <cinamo/Logger.h>
#include <cinamo/ClassUtil.h>
#include <cinamo/VectorMap.h>
#include <donut/Donut.h>
#include "../donut/WidgetObject.h"

namespace chisa {
using namespace cinamo;

namespace util {
class ParamSet;
class Param;
}
namespace tk {
class World;
class Widget;

template <typename WidgetKlass>
Handler<WidgetKlass> widgetConstructor(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* elem){
	return Handler<WidgetKlass>(new WidgetKlass(log, world, elem));
}

class WidgetFactory {
	DISABLE_COPY_AND_ASSIGN(WidgetFactory);
private:
	Logger& log_;
	HandlerW<World> world_;
	typedef std::function<Handler<Widget>(Logger& log, HandlerW<World> world, tinyxml2::XMLElement* elem)> ConstructorType;
	VectorMap<std::string, ConstructorType> widgetMap_;
	VectorMap<std::string, Handler<WidgetProvider> > demangledWidgetNameToDonutProviderMap_;
public: /* donut関連 */
	Handler<WidgetProvider> getProviderOf(Widget* me);
public:
	WidgetFactory(Logger& log, HandlerW<World> world);
	virtual ~WidgetFactory();
private: /* 登録 */
	void registerProvider(std::string const& demangledElementName, Handler<WidgetProvider> const& provider);
public:
	void registerWidget(std::string const& klass, ConstructorType const& func);
	template <typename WidgetKlass>
	void registerWidget(std::string const& klass) {
		static_assert(std::is_base_of<Widget, WidgetKlass>::value, "Please register provider for widget class.");
		this->registerWidget(klass, widgetConstructor<WidgetKlass>);
	}
	template <typename WidgetKlass>
	void registerProvider(Handler<WidgetProvider> const& provider)
	{
		static_assert(std::is_base_of<Widget, WidgetKlass>::value, "Please register provider for widget class.");
		this->registerProvider(::cinamo::demangle<WidgetKlass>(), provider);
	}
	void registerDonutProvider(Handler< ::donut::Heap> const& heap);
public: /* 作成 */
	Handler<Widget> createWidget(std::string const& klass, tinyxml2::XMLElement* elem);
private:
	Handler<Heap> heap_;
	class HeapLock final{
		STACK_OBJECT(HeapLock);
		DISABLE_COPY_AND_ASSIGN(HeapLock);
	public:
		WidgetFactory& self;
	public:
		HeapLock(WidgetFactory& self, Handler< ::donut::Heap> const& heap):self(self) {
			self.heap_ = heap;
		}
		~HeapLock(){
			self.heap_.reset();
		}
	};
};

}}
