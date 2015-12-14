/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/Exception.hpp>
#include "Patron.hpp"
#include "../World.hpp"

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
