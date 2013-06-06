/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <functional>
#include <tuple>
#include <cinamo/Handler.h>
#include <cinamo/functional/Maybe.h>
#include "../object/Object.h"


namespace donut {
using namespace cinamo;

class ReactiveNativeClosure : public HandlerBody<ReactiveNativeClosure>{
protected:
	ReactiveNativeClosure() = default;
public:
	virtual ~ReactiveNativeClosure() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const = 0;
};

class Heap;
template <typename __AntiSideEffect>
class ReactiveNativeClosureBaseT : public ReactiveNativeClosure{
public:
	typedef std::tuple<Handler<Object>, Maybe<__AntiSideEffect> > ResultType;
public:
	typedef std::function<ResultType(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg)> Signature;
	typedef ResultType(*Function)(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg);
private:
	Signature func_;
public:
	ReactiveNativeClosureBaseT(Signature f):func_(f){}
	virtual ~ReactiveNativeClosureBaseT() noexcept {};
public:
	virtual Handler<Object> apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const;
};

}

