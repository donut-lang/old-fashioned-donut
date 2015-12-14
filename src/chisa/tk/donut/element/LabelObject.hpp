/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../ElementGroupObject.hpp"
#include "../../element/Label.hpp"

namespace chisa {
namespace tk {

struct LabelSideEffect : public ElementSideEffect {
	enum {
		None=0,
		ChangeText,
	} op;
	std::string text;
	LabelSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		Super::serialize(arc);
	}
};

class LabelProvider;
class LabelObject : public ElementObjectBaseT<LabelProvider, LabelObject, Label, LabelSideEffect> {
public:
	LabelObject(LabelProvider* provider);
	virtual ~LabelObject() noexcept = default;
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

class LabelProvider : public ElementProviderBaseT<LabelProvider, LabelObject, Label, LabelSideEffect> {
public:
	LabelProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~LabelProvider() noexcept = default;
};

}}
