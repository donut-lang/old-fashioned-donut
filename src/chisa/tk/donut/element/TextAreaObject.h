/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../ElementGroupObject.h"
#include "../../element/TextArea.h"

namespace chisa {
namespace tk {

struct TextAreaSideEffect : public ElementSideEffect {
	enum {
		None=0,
		ChangeText,
		ChangeFrameColor
	} op;
	std::string text;
	gl::Color color;
	TextAreaSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		Super::serialize(arc);
	}
};

class TextAreaProvider;
class TextAreaObject : public ElementObjectBaseT<TextAreaProvider, TextAreaObject, TextArea, TextAreaSideEffect> {
public:
	TextAreaObject(TextAreaProvider* provider);
	virtual ~TextAreaObject() noexcept = default;
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class TextAreaProvider : public ElementProviderBaseT<TextAreaProvider, TextAreaObject, TextArea, TextAreaSideEffect> {
public:
	TextAreaProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~TextAreaProvider() noexcept = default;
};

}}
