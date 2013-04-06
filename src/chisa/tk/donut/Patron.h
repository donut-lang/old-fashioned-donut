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
