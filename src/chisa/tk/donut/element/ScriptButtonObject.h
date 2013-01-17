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
