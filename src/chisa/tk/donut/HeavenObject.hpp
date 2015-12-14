/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <string>
#include <cinamo/ClassUtil.h>
#include <donut/Donut.hpp>
#include <donut/object/ReactiveNativeObject.hpp>
#include "../Heaven.hpp"

namespace chisa {
using namespace cinamo;

namespace tk {
using namespace donut;

struct HeavenSideEffect{
	enum {
		AttatchAngel,
		DetatchAngel
	} op;
	Handler<Angel> attatchedAngel_;
	Handler<Angel> detatchedAngel_;
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class HeavenObject;
class HeavenProvider : public ::donut::ReactiveProviderBaseT<HeavenProvider, HeavenObject, HeavenSideEffect> {
private:
	HandlerW<Heaven> heaven_;
public:
	HeavenProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~HeavenProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

class HeavenObject : public ReactiveNativeObjectBaseT<HeavenObject, HeavenProvider, HeavenSideEffect> {
private:
	HandlerW<Heaven> heaven_;
public:
	HeavenObject(HeavenProvider* provider);
	virtual ~HeavenObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
public:
	Handler<Heaven> heaven() const;
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
};

}}
