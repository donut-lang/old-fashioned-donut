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
#include "AngelObject.h"
#include "ElementObject.h"
#include "../heaven/AngelElementTarget.h"
#include "../heaven/AngelWidgetTarget.h"
#include "AngelTargetObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("AngelObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

AngelProvider::AngelProvider(const Handler<Heap>& heap, const std::string& provname, const Handler<Heaven>& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
}

Handler<Heaven> AngelProvider::heaven() const
{
	return heaven_.lock();
}
//---------------------------------------------------------
AngelObject::AngelObject(AngelProvider* provider)
:ReactiveNativeObject(provider)
{
}

void AngelObject::bootstrap(const Handler<Heap>& heap, const Handler<Angel>& angel)
{
	ReactiveNativeObject::bootstrap(heap);
	this->angel_ = angel;
}

Handler<World> AngelObject::world() const
{
	return angel_->world();
}

Handler<Angel> AngelObject::angel() const
{
	return this->angel_;
}

/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
AngelProviderBaseT<ProviderT, ObjectT, AngelT, AntiT>::AngelProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:AngelProvider(heap, provname, heaven)
{
	this->registerPureNativeClosure("newElementTarget", [this](ObjectT* obj, std::string elementId){
		return obj->angel()->newElementTarget(elementId)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("newWidgetTarget", [this](ObjectT* obj, std::string elementId, std::string widgetGuide){
		return obj->angel()->newWidgetTarget(elementId, widgetGuide)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("findElementTarget", [this](ObjectT* obj, std::string elementId){
		return obj->angel()->findElementTarget(elementId)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("findWidgetTarget", [this](ObjectT* obj, std::string elementId, std::string widgetGuide){
		return obj->angel()->findWidgetTarget(elementId, widgetGuide)->donutObject(this->heap().lock());
	});

	this->registerReactiveNativeClosure("attatchTarget", [this](ObjectT* dangel, AngelTargetObject* dtarget) {
		Handler<AngelT> angel(dangel->angel());
		Handler<AngelTarget> target(dtarget->angelTarget());
		angel->attatchTarget(target);
		AntiT side_;
		AngelSideEffect& side = side_;
		side.op = AngelSideEffect::DetatchTarget;
		side.detatchedTarget_ = target;
		return std::make_tuple(dtarget, true, side_);
	});
	this->registerReactiveNativeClosure("detatchTarget", [this](ObjectT* dangel, AngelTargetObject* dtarget) {
		Handler<AngelT> angel(dangel->angel());
		Handler<AngelTarget> target(dtarget->angelTarget());
		angel->detatchTarget(target);
		AntiT side_;
		AngelSideEffect& side = side_;
		side.op = AngelSideEffect::AttatchTarget;
		side.attatchedTarget_ = target;
		return std::make_tuple(dtarget, true, side_);
	});
}

//---------------------------------------------------------

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::AngelObjectBaseT(ProviderT* provider)
:AngelObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::bootstrap(const Handler<Heap>& heap, Handler<Angel> const& angel)
{
	this->AngelObject::bootstrap(heap, angel);
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
Handler<AngelT> AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::angel() const
{
	return Handler<AngelT>::__internal__fromRawPointerWithoutCheck( static_cast<AngelT*>(AngelObject::angel().get()) );
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{

}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	AngelSideEffect& side = newAnti;
	AngelSideEffect const& old = val;
	switch(old.op){
	case AngelSideEffect::AttatchTarget:
		this->angel()->attatchTarget(old.attatchedTarget_);
		side.op = AngelSideEffect::DetatchTarget;
		side.detatchedTarget_ = old.attatchedTarget_;
		break;
	case AngelSideEffect::DetatchTarget:
		this->angel()->detatchTarget(old.detatchedTarget_);
		side.op = AngelSideEffect::AttatchTarget;
		side.attatchedTarget_ = old.detatchedTarget_;
		break;
	}
	return std::make_tuple(true, newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType AngelObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

/**********************************************************************************************************************
 * LoneAngel
 **********************************************************************************************************************/

LoneAngelProvider::LoneAngelProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "LoneAngelProvider", heaven)
{
}

//-------------------------------------------------------

LoneAngelObject::LoneAngelObject(LoneAngelProvider* provider)
:Super(provider)
{
}

void LoneAngelObject::bootstrap(const Handler<Heap>& heap, const Handler<LoneAngel>& angel)
{
	Super::bootstrap(heap, angel);
}

void LoneAngelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void LoneAngelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

typename LoneAngelObject::ResultType LoneAngelObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

typename LoneAngelObject::ResultType LoneAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

std::string LoneAngelObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(LoneAngelObject: %p)", this);
}


XValue LoneAngelObject::saveImpl(const Handler<Heap>& heap)
{
}

void LoneAngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

/**********************************************************************************************************************
 * TwinAngel
 **********************************************************************************************************************/

TwinAngelProvider::TwinAngelProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "TwinAngelProvider", heaven)
{
}

//-------------------------------------------------------

TwinAngelObject::TwinAngelObject(TwinAngelProvider* provider)
:Super(provider)
{
}

void TwinAngelObject::bootstrap(const Handler<Heap>& heap, const Handler<TwinAngel>& angel)
{
	Super::bootstrap(heap, angel);
}

void TwinAngelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void TwinAngelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

typename TwinAngelObject::ResultType TwinAngelObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

typename TwinAngelObject::ResultType TwinAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

std::string TwinAngelObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(TwinAngelObject: %p)", this);
}


XValue TwinAngelObject::saveImpl(const Handler<Heap>& heap)
{
}

void TwinAngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

}}
