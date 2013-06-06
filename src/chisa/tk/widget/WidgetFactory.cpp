/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>

#include "WidgetFactory.h"
#include "ImageWidget.h"
#include "ContentWidget.h"
#include "../World.h"

#include "../donut/widget/ContentObject.h"

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

	std::string const demangled(::cinamo::demangle(me));
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
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Provider for \"%s\" is already registered");
	}
	if( unlikely(!heap_) ) {
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Heap is not specified.");
	}
	heap_->registerProvider(provider);
}

Handler<Widget> WidgetFactory::createWidget(std::string const& klass, tinyxml2::XMLElement* elem)
{
	auto it = this->widgetMap_.find(klass);
	if(it == this->widgetMap_.end()){
		CINAMO_EXCEPTION(Exception, "[BUG] There is no widget named: \"%s\"", klass.c_str());
	}
	VectorMap<std::string, ConstructorType>::Pair const& pair = *it;
	return pair.second(log_, world_, elem);
}

void WidgetFactory::registerDonutProvider(Handler< ::donut::Heap> const& heap)
{
	HeapLock lock(*this, heap);
	Handler<World> world(this->world_.lock());
	if( unlikely(!world) ){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	this->registerProvider<ContentWidget>(Handler<WidgetProvider>(new ContentProvider(heap, world)));
}

}}
