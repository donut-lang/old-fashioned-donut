/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../ElementGroupObject.h"
#include "../../element/ScriptButton.h"

namespace chisa {
namespace tk {

struct ScriptButtonSideEffect : public ElementSideEffect {
	enum {
		None=0,
	} op;
	ScriptButtonSideEffect()
	:op(None)
	{
	}
	template <typename Arc>
	void serialize(Arc& arc) {
		Super::serialize(arc);
	}
};

class ScriptButtonProvider;
class ScriptButtonObject : public ElementObjectBaseT<ScriptButtonProvider, ScriptButtonObject, ScriptButton, ScriptButtonSideEffect> {
public:
	ScriptButtonObject(ScriptButtonProvider* provider);
	virtual ~ScriptButtonObject() noexcept = default;
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

class ScriptButtonProvider : public ElementProviderBaseT<ScriptButtonProvider, ScriptButtonObject, ScriptButton, ScriptButtonSideEffect> {
public:
	ScriptButtonProvider(Handler<Heap> const& heap, Handler<World> const& world);
	virtual ~ScriptButtonProvider() noexcept = default;
};

}}
