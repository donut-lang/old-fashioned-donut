/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../ElementGroupObject.h"
#include "../../element/SplitCombo.h"

namespace chisa {
namespace tk {

struct SplitComboSideEffect : public ElementGroupSideEffect {
	enum {
		None=0,
	} op;
	SplitComboSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		Super::serialize(arc);
	}
};

class SplitComboProvider;
class SplitComboObject : public ElementGroupObjectBaseT<SplitComboProvider, SplitComboObject, SplitCombo, SplitComboSideEffect> {
public:
	SplitComboObject(SplitComboProvider* provider);
	virtual ~SplitComboObject() noexcept = default;
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

class SplitComboProvider : public ElementGroupProviderBaseT<SplitComboProvider, SplitComboObject, SplitCombo, SplitComboSideEffect> {
public:
	SplitComboProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~SplitComboProvider() noexcept = default;
};

}}
