/**
 * Chisa
 * Copyright (C) 2013 psi
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
#include "ReactiveNativeClosureDecl.h"
#include "../object/ObjectDecl.h"
#include "../object/ReactiveNativeObjectDecl.h"
#include "../object/HeapDecl.h"

namespace donut {

template <typename __AntiSideEffect>
Handler<Object> ReactiveNativeClosureBaseT<__AntiSideEffect>::apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const {
	Handler<ReactiveNativeObjectAbstractT<__AntiSideEffect> > obj;
	if(!self->isObject() || !(obj = self.tryCast<ReactiveNativeObjectAbstractT<__AntiSideEffect> >())){
		DONUT_EXCEPTION(Exception, "[BUG] ReactiveNativeClosure must be applied only to ReactiveNativeObject.");
	}
	Handler<Object> result;
	bool succeed;
	__AntiSideEffect value;
	std::tie(result, succeed, value) = this->func_(heap, obj, arg);
	if( !succeed ) {
		heap->clock()->discardHistory();
	}else{
		obj->registerReaction(heap->clock()->now(), value);
	}
	return result;
}

}
