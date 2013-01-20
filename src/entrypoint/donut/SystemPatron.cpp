/**
 * Donut
 * Copyright (C) 2013 psi
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
