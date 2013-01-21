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
#include "../World.h"

namespace chisa {
namespace tk {

WidgetFactory::WidgetFactory(Logger& log, HandlerW<World> world)
:log_(log)
,world_(world)
{
	this->registerWidget<ImageWidget>("image");
	this->registerWidget<ContentWidget>("content");
}

WidgetFactory::~WidgetFactory()
{
}

void WidgetFactory::registerWidget(std::string const& klass, ConstructorType const& func)
{
	this->widgetMap_.update(klass, func);
}

Handler<WidgetProvider> WidgetFactory::getProviderOf(Widget* me)
{

	std::string const demangled(::tarte::demangle(me));
	VectorMap<std::string, Handler<WidgetProvider> >::Iterator it =
			this->demangledWidgetNameToDonutProviderMap_.find(demangled);
	if(it == this->demangledWidgetNameToDonutProviderMap_.end()){
		return Handler<WidgetProvider>();
	}
	VectorMap<std::string, Handler<WidgetProvider> >::Pair const& p(*it);
	return p.second;
}

void WidgetFactory::registerProvider(const std::string& demangledElementName, const Handler<WidgetProvider>& provider)
{
	if(!this->demangledWidgetNameToDonutProviderMap_.insert(demangledElementName, provider)){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. Provider for \"%s\" is already registered");
	}
	if( unlikely(!heap_) ) {
		TARTE_EXCEPTION(Exception, "[BUG] Oops. Heap is not specified.");
	}
	heap_->registerProvider(provider);
}

Handler<Widget> WidgetFactory::createWidget(std::string const& klass, tinyxml2::XMLElement* elem)
{
	auto it = this->widgetMap_.find(klass);
	if(it == this->widgetMap_.end()){
		TARTE_EXCEPTION(Exception, "[BUG] There is no widget named: \"%s\"", klass.c_str());
	}
	VectorMap<std::string, ConstructorType>::Pair const& pair = *it;
	return pair.second(log_, world_, elem);
}

void WidgetFactory::registerDonutProvider(Handler< ::donut::Heap> const& heap)
{
	HeapLock lock(*this, heap);
	Handler<World> world(this->world_.lock());
	if( unlikely(!world) ){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
}

}}
