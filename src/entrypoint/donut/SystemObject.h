/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <donut/object/Object.h>
#include <donut/object/ReactiveNativeObject.h>

namespace donut_cli {
using namespace cinamo;
using namespace donut;

class SystemProvider;
class SystemObject final: public ReactiveNativeObjectBaseT<SystemObject, SystemProvider, XValue> {
private:
	std::vector<std::string> args_;
public:
	SystemObject(HeapProvider* const provider);
	virtual ~SystemObject() noexcept = default;
protected:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
public:
	static Handler<Object> println(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
	static Handler<Object> print(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
	static Handler<Object> args(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
	static Handler<Object> readline(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
public:
	void bootstrap(Handler<Heap> const& heap);
};

}
