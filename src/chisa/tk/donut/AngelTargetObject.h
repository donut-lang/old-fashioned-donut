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
#include "../World.h"
#include "../Heaven.h"
#include "../Angel.h"
#include "../heaven/AngelElementTarget.h"
#include "../heaven/AngelWidgetTarget.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Servant
 **********************************************************************************************************************/

class AngelTargetProvider;
class AngelTargetObject : public ReactiveNativeObject {
private:
	Handler<AngelTarget> target_;
protected:
	AngelTargetObject(AngelTargetProvider* provider);
	virtual ~AngelTargetObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<AngelTarget> const& target);
public:
	Handler<World> world() const;
	Handler<AngelTarget> angelTarget() const;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
//public:
//	void onFutureDiscarded(Handler<Heap> const& heap);
//	void onHistoryDiscarded(Handler<Heap> const& heap);
//	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
//	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
//	XValue saveImpl( Handler<Heap> const& heap ) override final;
//	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class AngelTargetProvider : public ReactiveProvider {
private:
	HandlerW<Heaven> heaven_;
protected:
	AngelTargetProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~AngelTargetProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

/**********************************************************************************************************************
 * AngelElementTarget
 **********************************************************************************************************************/
struct AngelElementTargetSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class AngelElementTargetProvider;
class AngelElementTargetObject : public AngelTargetObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AngelElementTargetSideEffect, AngelElementTargetObject);
public:
	AngelElementTargetObject(AngelTargetProvider* provider);
	virtual ~AngelElementTargetObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<AngelElementTarget> const& AngelTarget);
public:
	Handler<World> world() const;
	Handler<AngelElementTarget> servant() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};
class AngelElementTargetProvider : public AngelTargetProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AngelElementTargetSideEffect, AngelElementTargetProvider);
private:
	HandlerW<Heaven> heaven_;
public:
	AngelElementTargetProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~AngelElementTargetProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new AngelElementTargetObject(this);
	}
};
/**********************************************************************************************************************
 * AngelWidgetTarget
 **********************************************************************************************************************/
struct AngelWidgetTargetSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class AngelWidgetTargetProvider;
class AngelWidgetTargetObject : public AngelTargetObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AngelWidgetTargetSideEffect, AngelWidgetTargetObject);
public:
	AngelWidgetTargetObject(AngelTargetProvider* provider);
	virtual ~AngelWidgetTargetObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<AngelWidgetTarget> const& AngelTarget);
public:
	Handler<World> world() const;
	Handler<AngelWidgetTarget> angelTarget() const;
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};
class AngelWidgetTargetProvider : public AngelTargetProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AngelWidgetTargetSideEffect, AngelWidgetTargetProvider);
private:
	HandlerW<Heaven> heaven_;
public:
	AngelWidgetTargetProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~AngelWidgetTargetProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new AngelWidgetTargetObject(this);
	}
};


}}
