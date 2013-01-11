/**
 * Chisa
 * Copyright (C) 2012 psi
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
#include <string>
#include <tarte/ClassUtil.h>
#include <donut/Donut.h>
#include <donut/object/ReactiveNativeObject.h>
#include "../Heaven.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

struct HeavenSideEffect{
	enum {
		AttatchAngel,
		DetatchAngel
	} op;
	Handler<Angel> attatchedAngel_;
	Handler<Angel> detatchedAngel_;
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class HeavenObject;
class HeavenProvider : public ::donut::ReactiveProviderBaseT<HeavenProvider, HeavenObject, HeavenSideEffect> {
private:
	HandlerW<Heaven> heaven_;
public:
	HeavenProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~HeavenProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

class HeavenObject : public ReactiveNativeObjectBaseT<HeavenObject, HeavenProvider, HeavenSideEffect> {
private:
	HandlerW<Heaven> heaven_;
public:
	HeavenObject(HeavenProvider* provider);
	virtual ~HeavenObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
public:
	Handler<Heaven> heaven() const;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
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

}}
