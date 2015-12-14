/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ProviderDecl.hpp"
#include "../object/HeapDecl.hpp"

namespace donut {


//関数ポインタ版
template <> bool Provider::registerPureNativeClosure<PureNativeClosureObject::Function> ( std::string const& name, PureNativeClosureObject::Function f);
// std::function版
template <> bool Provider::registerPureNativeClosure<PureNativeClosureObject::Signature>( std::string const& name, PureNativeClosureObject::Signature f);

}


namespace donut {
namespace internal {



}}

namespace donut {

template <typename __AntiSideEffect>
void ReactiveProviderAspectT<__AntiSideEffect>::bootstrap(ReactiveProvider& self) {
	Handler<Heap> heap = self.heap().lock();
	if( unlikely(!heap) ){
		DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
	}
	for( std::pair<std::string, typename ReactiveNativeClosureBaseT<__AntiSideEffect>::Signature> const& p : this->reactiveNativeClosures_ ){
		self.prototype()->set(
				heap,
				p.first,
				heap->createReactiveNativeClosureObject< __AntiSideEffect >(self.name(), p.first, p.second));
	}
}

template <typename ObjectT, typename ProviderT, typename... Args>
inline Handler<ObjectT> HeapProvider::newInstance(ProviderT* self, Args... args)
{
	Handler<Heap> heap(self->heap().lock());
	if( unlikely(!heap) ) {
		DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
	}
	return heap->createObject<ObjectT>(self, args...);
}

}
