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

#include "LabelObject.h"

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
	return std::tuple<bool, AntiSideEffect>(true, anti);
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
		return std::make_tuple(std::nullptr_t(), true, anti);
	});
}

}}
