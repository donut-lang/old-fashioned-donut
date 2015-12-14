/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "HeapDecl.hpp"
#include "../object/ReactiveNativeClosureObject.hpp"

namespace donut {
using namespace cinamo;

template <typename __AntiSideEffect>
inline Handler<ReactiveNativeClosureObject> Heap::createReactiveNativeClosureObject(std::string const& objectProviderName, std::string const& closureName, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature f) {
	Handler<ReactiveNativeClosureObject> obj(this->reactiveNativeClosureProvider_->newInstance<__AntiSideEffect>(objectProviderName, closureName, f));
	this->registerObject(obj);

	return obj;
}

}
