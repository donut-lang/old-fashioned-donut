/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "TextAreaObject.hpp"

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
		anti.op = AntiSideEffect::ChangeText;
		anti.text = element()->text();
		element()->text(val.text);
		break;
	case AntiSideEffect::ChangeFrameColor:
		anti.op = AntiSideEffect::ChangeFrameColor;
		anti.color = element()->frameColor();
		element()->frameColor(val.color);
		break;
	case AntiSideEffect::None:
		break;
	}
	return Just<AntiSideEffect>(anti);
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
	this->registerReactiveNativeClosure("setFrameColor", [](TextAreaObject* obj, std::string c){
		AntiSideEffect anti;
		anti.op=AntiSideEffect::ChangeFrameColor;
		anti.color = obj->element()->frameColor();
		obj->element()->frameColor( gl::Color::fromString(c) );
		return std::make_tuple(std::nullptr_t(), Just<AntiSideEffect>(anti));
	});
}

}}
