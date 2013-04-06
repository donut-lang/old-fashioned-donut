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

#include <donut/Exception.h>
#include "Patron.h"
#include "../World.h"

namespace chisa {
using namespace cinamo;

namespace tk {

const static std::string TAG("Patron");

Patron::Patron(Handler<Universe> const& universe, Handler<World> const& world)
:universe_(universe)
,world_(world)
{
}

void Patron::onRegisterProvider(Handler< ::donut::Heap> const& heap)
{
	Handler<Universe> universe = this->universe_.lock();
	if( unlikely(!universe) ){
		DONUT_EXCEPTION(Exception, "[BUG] Failed to lock universe.");
	}
	Handler<World> world = this->world_.lock();
	if( unlikely(!world) ){
		DONUT_EXCEPTION(Exception, "[BUG] Failed to lock world.");
	}
	//world
	heap->registerProvider(this->worldProvider_ = Handler<WorldProvider>( new WorldProvider(heap, world) ));
	// heaven
	heap->registerProvider(this->heavenProvider_ = Handler<HeavenProvider>( new HeavenProvider(heap, world->heaven()) ));
	// angel
	heap->registerProvider(this->loneAngelProvider_ = Handler<LoneAngelProvider>( new LoneAngelProvider(heap, world->heaven()) ));
	heap->registerProvider(this->twinAngelProvider_ = Handler<TwinAngelProvider>( new TwinAngelProvider(heap, world->heaven()) ));
	// angel target
	heap->registerProvider(this->angelElementTargetProvider_ = Handler<AngelElementTargetProvider>( new AngelElementTargetProvider(heap, world->heaven()) ));
	heap->registerProvider(this->angelWidgetTargetProvider_ = Handler<AngelWidgetTargetProvider>( new AngelWidgetTargetProvider(heap, world->heaven()) ));
	// servant
	heap->registerProvider(this->haloServantProvider_ = Handler<HaloServantProvider>( new HaloServantProvider(heap, world->heaven()) ));
	heap->registerProvider(this->contentUnderlineServantProvider_ = Handler<ContentUnderlineServantProvider>( new ContentUnderlineServantProvider(heap, world->heaven()) ));
	heap->registerProvider(this->elementServantProvider_ = Handler<ElementServantProvider>( new ElementServantProvider(heap, world->heaven()) ));
	//ウィジットのプロバイダを設定
	world->elementFactory()->registerDonutProvider(heap);
	world->widgetFactory()->registerDonutProvider(heap);

	universe->hexe()->registerGeistProvider(heap);

}

void Patron::onGlobalObjectInitialized(Handler< ::donut::Heap> const& heap)
{
	heap->setGlobalObject("World", this->world().lock()->donutObject(heap));
}

}}


