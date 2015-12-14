/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ProviderDecl.hpp"

namespace donut {

class PureNativeClosureObject;
class PureNativeClosureProvider final : public HeapProviderBaseT<PureNativeClosureProvider, PureNativeClosureObject> {
public:
	PureNativeClosureProvider( Handler<Heap> const& heap );
	virtual ~PureNativeClosureProvider() noexcept = default;
};

class ReactiveNativeClosureObject;
class ReactiveNativeClosureProvider final : public HeapProviderBaseT<ReactiveNativeClosureProvider, ReactiveNativeClosureObject> {
public:
	ReactiveNativeClosureProvider( Handler<Heap> const& heap );
	virtual ~ReactiveNativeClosureProvider() noexcept = default;
public:
	template <typename __AntisideEffect, typename... Args> inline
	Handler<ReactiveNativeClosureObject> newInstance(Args... args) {
		Handler<ReactiveNativeClosureObject> t ( new ReactiveNativeClosureObject( static_cast<ReactiveNativeClosureProvider*>(this) ) );
		t->bootstrap<__AntisideEffect>(args...);
		return t;
	}
};

}
