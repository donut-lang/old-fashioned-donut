/**
/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "ScriptButtonObject.h"

namespace chisa {
namespace tk {

ScriptButtonObject::ScriptButtonObject(ScriptButtonProvider* provider)
:Super(provider)
{
}

void ScriptButtonObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void ScriptButtonObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

typename ScriptButtonObject::ResultType ScriptButtonObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::None:
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. you have to process none before");
		break;
	}
	return Just<AntiSideEffect>(anti);
}

typename ScriptButtonObject::ResultType ScriptButtonObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename ScriptButtonObject::ResultType ScriptButtonObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue ScriptButtonObject::saveImpl(const Handler<Heap>& heap)
{
}

void ScriptButtonObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//---------------------------------------------------------------------------------------

ScriptButtonProvider::ScriptButtonProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
}

}}
