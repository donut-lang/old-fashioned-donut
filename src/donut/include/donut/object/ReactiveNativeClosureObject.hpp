/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ReactiveNativeClosureObjectDecl.hpp"
#include "../native/ReactiveNativeClosure.hpp"

namespace donut {

template <typename __AntiSideEffect>
void ReactiveNativeClosureObject::bootstrap( std::string const& objectProviderName, std::string const& closureName, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature f )
{
	this->NativeClosureObject::bootstrap(objectProviderName, closureName);
	this->spirit_ = Handler<ReactiveNativeClosure>(new ReactiveNativeClosureBaseT<__AntiSideEffect>(f));
}

}
