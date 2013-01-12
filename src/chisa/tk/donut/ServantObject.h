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
#include "../heaven/ElementServant.h"
#include "../heaven/HaloServant.h"

namespace chisa {
using namespace tarte;

namespace tk {
using namespace donut;

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/
struct ServantSideEffect{
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

class ServantProvider;
class ServantObject : public ReactiveNativeObject {
private:
	Handler<Servant> target_;
protected:
	ServantObject(ServantProvider* provider);
	virtual ~ServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<Servant> const& target);
public:
	Handler<World> world() const;
	Handler<Servant> angelTarget() const;
//public:
//	void onFutureDiscarded(Handler<Heap> const& heap);
//	void onHistoryDiscarded(Handler<Heap> const& heap);
//	ResultType onBack(Handler<Heap> const& heap, AntiSideEffect const& val);
//	ResultType onForward(Handler<Heap> const& heap, AntiSideEffect const& val);
//	XValue saveImpl( Handler<Heap> const& heap ) override final;
//	void loadImpl( Handler<Heap> const& heap, XValue const& data ) override final;
};

class ServantProvider : public ReactiveProvider {
private:
	HandlerW<Heaven> heaven_;
protected:
	ServantProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~ServantProvider() noexcept = default;
public:
	Handler<Heaven> heaven() const;
};

/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/
template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
class ServantObjectBaseT : public ServantObject {
	INJECT_REACTIVE_OBJECT_ASPECT(AntiT, ObjectT)
protected:
	typedef ServantObjectBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	ServantObjectBaseT(ProviderT* provider);
	virtual ~ServantObjectBaseT() noexcept = default;
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
class ServantProviderBaseT : public ServantProvider {
	INJECT_REACTIVE_PROVIDER_ASPECT(AntiT, ServantProvider)
protected:
	typedef ServantProviderBaseT<ProviderT, ObjectT, TargetT, AntiT> Super;
	ServantProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven);
	virtual ~ServantProviderBaseT() noexcept = default;
};

/**********************************************************************************************************************
 * ElementServant
 **********************************************************************************************************************/
struct ElementServantSideEffect : public ServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class ElementServantProvider;
class ElementServantObject : public ServantObjectBaseT<ElementServantProvider, ElementServantObject, ElementServant, ElementServantSideEffect> {
public:
	ElementServantObject(ElementServantProvider* provider);
	virtual ~ElementServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<ElementServant> const& Servant);
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

class ElementServantProvider : public ServantProviderBaseT<ElementServantProvider, ElementServantObject, ElementServant, ElementServantSideEffect> {
public:
	ElementServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~ElementServantProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new ElementServantObject(this);
	}
};
/**********************************************************************************************************************
 * HaloServant
 **********************************************************************************************************************/
struct HaloServantSideEffect : public ServantSideEffect{
	template <typename Arc>
	void serialize(Arc& arc) {

	}
};

class HaloServantProvider;
class HaloServantObject : public ServantObjectBaseT<HaloServantProvider, HaloServantObject, HaloServant, HaloServantSideEffect> {
public:
	HaloServantObject(HaloServantProvider* provider);
	virtual ~HaloServantObject() noexcept = default;
public:
	void bootstrap(Handler<Heap> const& heap, Handler<HaloServant> const& Servant);
public:
	Handler<World> world() const;
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
class HaloServantProvider : public ServantProviderBaseT<HaloServantProvider, HaloServantObject, HaloServant, HaloServantSideEffect> {
public:
	HaloServantProvider(Handler<Heap> const& heap, Handler<Heaven> const& heaven);
	virtual ~HaloServantProvider() noexcept = default;
private:
	virtual ::donut::HeapObject* __internal__createInstanceForLoading() override final {
		return new HaloServantObject(this);
	}
};


}}
