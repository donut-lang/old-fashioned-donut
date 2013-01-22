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
#include <string>
#include <tarte/ClassUtil.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../World.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

struct WorldSideEffect{
	enum {
		CreateAngel,
		RemoveAngel
	} op;
	union {

	};
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class WorldObject;
class WorldProvider : public ::donut::ReactiveProviderBaseT<WorldProvider, WorldObject, WorldSideEffect> {
private:
	HandlerW<World> world_;
public:
	WorldProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~WorldProvider() noexcept = default;
public:
	Handler<World> world() const;
};

class WorldObject : public ReactiveNativeObjectBaseT<WorldObject, WorldProvider, WorldSideEffect> {
private:
	HandlerW<World> world_;
public:
	WorldObject(WorldProvider* provider);
	virtual ~WorldObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<World> const& world);
public:
	Handler<World> world() const;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	static Handler< ::donut::Object> heaven(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& args);
	Handler<ElementObject> findElementById(std::string const& id);
	Handler<WidgetObject> findWidgetById(std::string const& widgetid);
	std::tuple<Handler<ElementObject>, bool, WorldSideEffect> pushElement(std::string const& elementId);

};

}}
