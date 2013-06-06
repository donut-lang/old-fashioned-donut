/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <cinamo/Handler.h>
#include <donut/Patron.h>
#include "HeavenObject.h"
#include "WorldObject.h"
#include "AngelObject.h"
#include "AngelTargetObject.h"
#include "ServantObject.h"

namespace chisa {
using namespace cinamo;

namespace tk {
class World;

class Patron : public donut::Patron {
	HandlerW<Universe> universe_;
	HandlerW<World> world_;
private:
	Handler<WorldProvider> worldProvider_;
	Handler<HeavenProvider> heavenProvider_;
	Handler<AngelElementTargetProvider> angelElementTargetProvider_;
	Handler<AngelWidgetTargetProvider> angelWidgetTargetProvider_;
	Handler<TwinAngelProvider> twinAngelProvider_;
	Handler<LoneAngelProvider> loneAngelProvider_;
	Handler<ElementServantProvider> elementServantProvider_;
	Handler<HaloServantProvider> haloServantProvider_;
	Handler<ContentUnderlineServantProvider> contentUnderlineServantProvider_;
public:
	Patron(Handler<Universe> const& universe, Handler<World> const& world);
	virtual ~Patron() noexcept = default;
public:
	virtual void onRegisterProvider(Handler< ::donut::Heap> const& heap) override final;
	virtual void onGlobalObjectInitialized(Handler< ::donut::Heap> const& heap) override final;

	const Handler<WorldProvider>& worldProvider() const { return worldProvider_; }
	const Handler<HeavenProvider>& heavenProvider() const { return heavenProvider_; }
	const Handler<AngelElementTargetProvider>& angelElementTargetProvider() const { return angelElementTargetProvider_; }
	const Handler<AngelWidgetTargetProvider>& angelWidgetTargetProvider() const { return angelWidgetTargetProvider_; }
	const Handler<ElementServantProvider>& elementServantProvider() const { return elementServantProvider_; }
	const Handler<HaloServantProvider>& haloServantProvider() const { return haloServantProvider_; }
	const Handler<ContentUnderlineServantProvider>& contentUnderlineServantProvider() const { return contentUnderlineServantProvider_; }
	const Handler<LoneAngelProvider>& loneAngelProvider() const { return loneAngelProvider_; }
	const Handler<TwinAngelProvider>& twinAngelProvider() const { return twinAngelProvider_; }
	const HandlerW<World>& world() const { return world_; }
};

}}
