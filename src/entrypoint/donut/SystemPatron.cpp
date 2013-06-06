/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include "SystemPatron.h"
#include "SystemObject.h"

namespace donut_cli {
SystemPatron::SystemPatron(int argc, char** argv): argc_(argc), argv_(argv){

}

void SystemPatron::onRegisterProvider(Handler<Heap> const& heap)
{
	heap->registerProvider(systemProvider_ = Handler<SystemProvider>(new SystemProvider(heap, argc_, argv_)));
}
void SystemPatron::onGlobalObjectInitialized(Handler<Heap> const& heap)
{
	heap->setGlobalObject("System", systemProvider_->newInstance(heap));
}

}
