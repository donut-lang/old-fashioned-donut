/**
 * Donut
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
#include "ProviderDecl.h"
#include "../object/HeapDecl.h"

namespace donut {


//関数ポインタ版
template <> bool Provider::registerPureNativeClosure<PureNativeClosureObject::Function> ( std::string const& name, PureNativeClosureObject::Function f);
// std::function版
template <> bool Provider::registerPureNativeClosure<PureNativeClosureObject::Signature>( std::string const& name, PureNativeClosureObject::Signature f);

}

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

}
