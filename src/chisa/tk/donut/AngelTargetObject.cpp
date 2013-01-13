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

#include <functional>
#include "AngelTargetObject.h"
#include "ElementObject.h"
#include "../heaven/HaloServant.h"
#include "../heaven/ElementServant.h"
#include "ServantObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("AngelTargetObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

AngelTargetProvider::AngelTargetProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
	this->registerPureNativeClosure("newHaloServant", [this]( AngelTargetObject* target, std::string color ){
		return target->angelTarget()->newHaloServant(gl::Color::fromString(color))->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("newElementServant", [this]( AngelTargetObject* target, std::string elemId )->Handler<Object>{
		return target->angelTarget()->newElementServant(elemId)->donutObject(this->heap().lock());
	});
}

Handler<Heaven> AngelTargetProvider::heaven() const
{
	return heaven_.lock();
}

//---------------------------------------------------------

AngelTargetObject::AngelTargetObject(AngelTargetProvider* provider)
:ReactiveNativeObject(provider)
{
}

void AngelTargetObject::bootstrap(const Handler<Heap>& heap, Handler<AngelTarget> const& angelTarget)
{
	this->ReactiveNativeObject::bootstrap(heap);
	this->target_ = angelTarget;
}

Handler<World> AngelTargetObject::world() const
{
	return target_->world();
}

Handler<AngelTarget> AngelTargetObject::angelTarget() const
{
	return this->target_;
}


/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/
template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
AngelTargetProviderBaseT<ProviderT, ObjectT, TargetT, AntiT>::AngelTargetProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:AngelTargetProvider(heap, provname, heaven)
{
	this->registerReactiveNativeClosure("attatchServant", [this](AngelTargetObject* target, ServantObject* servant) {
		AntiT anti;
		AngelTargetSideEffect& side = anti;
		side.op = AngelTargetSideEffect::DetatchServant;
		side.detatchedServant = servant->servant();
		target->angelTarget()->attatchServant(servant->servant());
		return std::make_tuple(servant, true, anti);
	});

	this->registerReactiveNativeClosure("detatchServant", [this](AngelTargetObject* target, ServantObject* servant) {
		AntiT anti;
		AngelTargetSideEffect& side = anti;
		side.op = AngelTargetSideEffect::AttatchServant;
		side.attatchedServant = servant->servant();
		return std::make_tuple(servant, true, anti);
	});
}

//---------------------------------------------------------

template<typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
inline AngelTargetObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::AngelTargetObjectBaseT(ProviderT* provider)
:AngelTargetObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
Handler<TargetT> AngelTargetObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::angelTarget() const
{
	return Handler<TargetT>::__internal__fromRawPointerWithoutCheck( static_cast<TargetT*>(AngelTargetObject::angelTarget().get()) );
}


template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
void AngelTargetObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::bootstrap(const Handler<Heap>& heap, const Handler<TargetT>& angelTarget)
{
	this->AngelTargetObject::bootstrap(heap, angelTarget);
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{

}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	AngelTargetSideEffect& side = newAnti;
	AngelTargetSideEffect const& old = val;
	switch(old.op){
	case AngelTargetSideEffect::AttatchServant:
		this->angelTarget()->attatchServant( old.attatchedServant );
		side.op = AngelTargetSideEffect::DetatchServant;
		side.detatchedServant = old.attatchedServant;
		break;
	case AngelTargetSideEffect::DetatchServant:
		this->angelTarget()->detatchServant( old.detatchedServant );
		side.op = AngelTargetSideEffect::AttatchServant;
		side.attatchedServant = old.detatchedServant;
		break;
	}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelTargetObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

/**********************************************************************************************************************
 * Element
 **********************************************************************************************************************/

AngelElementTargetProvider::AngelElementTargetProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "AngelElementTarget", heaven)
{
}

//---------------------------------------------------------

AngelElementTargetObject::AngelElementTargetObject(AngelElementTargetProvider* provider)
:Super(provider)
{
}

void AngelElementTargetObject::bootstrap(const Handler<Heap>& heap, const Handler<AngelElementTarget>& angeltarget)
{
	Super::bootstrap(heap, angeltarget);
}

void AngelElementTargetObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void AngelElementTargetObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

AngelElementTargetObject::ResultType AngelElementTargetObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

AngelElementTargetObject::ResultType AngelElementTargetObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

XValue AngelElementTargetObject::saveImpl(const Handler<Heap>& heap)
{
}

void AngelElementTargetObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::string AngelElementTargetObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(AngelElementTargetObject: %p)", this);
}


/**********************************************************************************************************************
 * Widget
 **********************************************************************************************************************/

AngelWidgetTargetProvider::AngelWidgetTargetProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "AngelWidgetTarget", heaven)
{
}

//---------------------------------------------------------

AngelWidgetTargetObject::AngelWidgetTargetObject(AngelWidgetTargetProvider* provider)
:Super(provider)
{
}

void AngelWidgetTargetObject::bootstrap(const Handler<Heap>& heap, const Handler<AngelWidgetTarget>& angelTarget)
{
	Super::bootstrap(heap, angelTarget);
}

void AngelWidgetTargetObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void AngelWidgetTargetObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

AngelWidgetTargetObject::ResultType AngelWidgetTargetObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

AngelWidgetTargetObject::ResultType AngelWidgetTargetObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

XValue AngelWidgetTargetObject::saveImpl(const Handler<Heap>& heap)
{
}

void AngelWidgetTargetObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::string AngelWidgetTargetObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(AngelWidgetTargetObject: %p)", this);
}

}}


