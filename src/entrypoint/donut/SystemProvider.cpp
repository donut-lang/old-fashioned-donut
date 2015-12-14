/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/Heap.hpp>
#include "SystemProvider.hpp"
#include "SystemObject.hpp"

namespace donut_cli {

static const std::string TAG("SystemProvider");

SystemProvider::SystemProvider(Handler<Heap> const& heap, int argc, char** argv)
:Super(heap, "System")
,args_(argv, argv+argc)
{
	this->registerPureNativeClosure("println", SystemObject::println);
	this->registerPureNativeClosure("print", SystemObject::print);
	this->registerPureNativeClosure("args", SystemObject::args);
	this->registerPureNativeClosure("readline", SystemObject::readline);
}

}
