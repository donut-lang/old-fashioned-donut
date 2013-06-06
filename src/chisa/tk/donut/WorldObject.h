/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <cinamo/ClassUtil.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../World.h"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

struct WorldSideEffect{
	enum {
		PushElement,
		PopElement
	} op;
	union {

	};
	Handler<Element> elm;
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
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
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
	std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> > pushElement(std::string const& elementId);
	std::tuple<Handler<ElementObject>, Maybe<WorldSideEffect> > popElement();

};

}}
