/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "SplitComboObject.hpp"

namespace chisa {
namespace tk {

SplitComboObject::SplitComboObject(SplitComboProvider* provider)
:Super(provider)
{
}

void SplitComboObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void SplitComboObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

typename SplitComboObject::ResultType SplitComboObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	AntiSideEffect anti;
	//switch(val.op){

	//}
	return Just<AntiSideEffect>(anti);
}

typename SplitComboObject::ResultType SplitComboObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename SplitComboObject::ResultType SplitComboObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue SplitComboObject::saveImpl(const Handler<Heap>& heap)
{
}

void SplitComboObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//---------------------------------------------------------------------------------------

SplitComboProvider::SplitComboProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
}

}}
