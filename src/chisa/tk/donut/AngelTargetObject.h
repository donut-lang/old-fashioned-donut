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
 * Base
 **********************************************************************************************************************/
struct AngelTargetSideEffect{
	enum {
		AttatchServant,
		DetatchServant,
	}op;
	Handler<Servant> attatchedServant;
	Handler<Servant> detatchedServant;
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

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
 * BaseT
 **********************************************************************************************************************/
template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
class AngelTargetObjectBaseT : public AngelTargetObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT)
protected:
	typedef AngelTargetObjectBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	AngelTargetObjectBaseT(ProviderT* provider);
	virtual ~AngelTargetObjectBaseT() noexcept = default;
protected:
	void bootstrap(const Handler<Heap>& heap, const Handler<TargetT>& angelTarget);
public:
	Handler<TargetT> angelTarget() const;
private:
	ResultType execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
};

template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
class AngelTargetProviderBaseT : public AngelTargetProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, AngelTargetProvider)
protected:
	typedef AngelTargetProviderBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	AngelTargetProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~AngelTargetProviderBaseT() noexcept = default;
};

/**********************************************************************************************************************
 * AngelElementTarget
 **********************************************************************************************************************/
struct AngelElementTargetSideEffect : public AngelTargetSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class AngelElementTargetProvider;
class AngelElementTargetObject : public AngelTargetObjectBaseT<AngelElementTargetProvider, AngelElementTargetObject, AngelElementTarget, AngelElementTargetSideEffect> {
public:
	AngelElementTargetObject(AngelElementTargetProvider* provider);
	virtual ~AngelElementTargetObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<AngelElementTarget> const& AngelTarget);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
};

class AngelElementTargetProvider : public AngelTargetProviderBaseT<AngelElementTargetProvider, AngelElementTargetObject, AngelElementTarget, AngelElementTargetSideEffect> {
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
struct AngelWidgetTargetSideEffect : public AngelTargetSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class AngelWidgetTargetProvider;
class AngelWidgetTargetObject : public AngelTargetObjectBaseT<AngelWidgetTargetProvider, AngelWidgetTargetObject, AngelWidgetTarget, AngelWidgetTargetSideEffect> {
public:
	AngelWidgetTargetObject(AngelWidgetTargetProvider* provider);
	virtual ~AngelWidgetTargetObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<AngelWidgetTarget> const& AngelTarget);
public:
	void onFutureDiscarded(Handler<Heap> const& heap);
	void onHistoryDiscarded(Handler<Heap> const& heap);
	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
	XValue saveImpl( Handler<Heap> const& heap ) override final;
	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
private:
	virtual std::string reprImpl(Handler<Heap> const& heap) const override final;
};
class AngelWidgetTargetProvider : public AngelTargetProviderBaseT<AngelWidgetTargetProvider, AngelWidgetTargetObject, AngelWidgetTarget, AngelWidgetTargetSideEffect> {
public:
	AngelWidgetTargetProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~AngelWidgetTargetProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new AngelWidgetTargetObject(this);
	}
};


}}
