/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "LabelObject.hpp"

namespace chisa {
namespace tk {

LabelObject::LabelObject(LabelProvider* provider)
:Super(provider)
{
}

void LabelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void LabelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

typename LabelObject::ResultType LabelObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::ChangeText:
		anti.op = AntiSideEffect::ChangeText;
		anti.text = element()->text();
		element()->text(val.text);
		break;
	case AntiSideEffect::None:
		break;
	}
	return Just<AntiSideEffect>(anti);
}

typename LabelObject::ResultType LabelObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename LabelObject::ResultType LabelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue LabelObject::saveImpl(const Handler<Heap>& heap)
{
}

void LabelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//---------------------------------------------------------------------------------------

LabelProvider::LabelProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
	this->registerPureNativeClosure("getText", [](LabelObject* obj){
		return obj->element()->text();
	});
	this->registerReactiveNativeClosure("setText", [](LabelObject* obj, std::string c){
		AntiSideEffect anti;
		anti.op=AntiSideEffect::ChangeText;
		anti.text = obj->element()->text();
		obj->element()->text( c );
		return std::make_tuple(std::nullptr_t(), Just<AntiSideEffect>(anti));
	});
}

}}
