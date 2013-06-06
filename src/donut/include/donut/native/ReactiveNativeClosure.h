/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ReactiveNativeClosureDecl.h"
#include "../object/ObjectDecl.h"
#include "../object/ReactiveNativeObjectDecl.h"
#include "../object/HeapDecl.h"

namespace donut {

template <typename __AntiSideEffect>
Handler<Object> ReactiveNativeClosureBaseT<__AntiSideEffect>::apply(Handler<Heap> const& heap, Handler<Object> const& self, std::vector<Handler<Object> > const& arg) const {
	Handler<ReactiveNativeObject> obj;
	if(!self->isObject() || !(obj = self.tryCast<ReactiveNativeObject>())){
		DONUT_EXCEPTION(Exception, "[BUG] ReactiveNativeClosure must be applied only to ReactiveNativeObject.\nActual type: %s",
				demangle(self.get()).c_str());
	}
	ReactionRecordT<__AntiSideEffect>* record = dynamic_cast<ReactionRecordT<__AntiSideEffect>*>(obj->reactionRecorde());
	if( !record ){
		DONUT_EXCEPTION(Exception, "[BUG] AntiEffect type does not match. SelfType: %s\n\tAntiSideEffect type: \n\t\trequested: %s\n\t\t actual: %s",
				demangle(self.get()).c_str(), demangle(obj->reactionRecorde()).c_str(), demangle<__AntiSideEffect>().c_str());
	}
	ReactiveNativeClosureBaseT<__AntiSideEffect>::ResultType res ( this->func_(heap, obj, arg) );
	if( std::get<1>(res).isNothing ) {
		heap->clock()->discardHistory();
	}else{
		record->registerReaction(obj, heap->clock()->now(), std::get<1>(res).value() );
	}
	return std::get<0>(res);
}

}
