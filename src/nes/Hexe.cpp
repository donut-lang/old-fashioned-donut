/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Hexe.h"
#include "widget/NesScreenWidget.h"
#include "widget/NesMemoryWidget.h"
#include "widget/NesMemoryCompare.h"
#include "widget/NesTraceWidget.h"
#include "NesGeist.h"

namespace nes {

static std::string TAG("nes::Hexe");

Hexe::Hexe(chisa::Logger& log, std::string const& basepath)
:chisa::Hexe(log, basepath)
{

}

Hexe::~Hexe() noexcept
{
	log().d(TAG, "Shutting down...");
}

void Hexe::registerGeistProvider( ::cinamo::Handler< ::donut::Heap> const& heap )
{
	heap->registerProvider(this->nesGeistProvider_ = Handler<NesGeistProvider>(new NesGeistProvider(heap)));
}

void Hexe::registerWidgets(chisa::tk::WidgetFactory& factory)
{
	factory.registerWidget<nes::NesScreenWidget>("nes-screen");
	factory.registerWidget<nes::NesMemoryWidget>("nes-memory");
	factory.registerWidget<nes::NesMemoryCompare>("nes-compare");
	factory.registerWidget<nes::NesTraceWidget>("nes-trace");
}

void Hexe::registerElements(chisa::tk::ElementFactory& factory)
{
}

chisa::Handler<chisa::WorldGeist> Hexe::invokeWorldGeist(chisa::HandlerW<chisa::tk::World> world, std::string const& nameOfGeist)
{
	if(nameOfGeist == "nes"){
		return chisa::Handler<chisa::WorldGeist>(new NesGeist(log(), self(), world));
	}
	return chisa::Handler<chisa::WorldGeist>();
}

}
