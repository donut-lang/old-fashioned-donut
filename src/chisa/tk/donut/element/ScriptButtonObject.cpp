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
	return std::tuple<bool, AntiSideEffect>(true, anti);
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
