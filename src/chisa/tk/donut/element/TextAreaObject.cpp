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

#include "TextAreaObject.h"

namespace chisa {
namespace tk {

TextAreaObject::TextAreaObject(TextAreaProvider* provider)
:Super(provider)
{
}

void TextAreaObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void TextAreaObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

typename TextAreaObject::ResultType TextAreaObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::ChangeText:
		break;
	}
	return std::tuple<bool, AntiSideEffect>(true, anti);
}

typename TextAreaObject::ResultType TextAreaObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename TextAreaObject::ResultType TextAreaObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue TextAreaObject::saveImpl(const Handler<Heap>& heap)
{
}

void TextAreaObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//---------------------------------------------------------------------------------------

TextAreaProvider::TextAreaProvider(Handler<Heap> const& heap, Handler<World> const& world)
:Super(heap, world)
{
	this->registerPureNativeClosure("setText", [](TextAreaObject* obj, std::string str){
		obj->element()->text(str);
		return str;
	});
	this->registerPureNativeClosure("getText", [](TextAreaObject* obj){
		return obj->element()->text();
	});
}

}}
