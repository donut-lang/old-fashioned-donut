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
#include "ServantObject.h"
#include "ElementObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("ServantObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

ServantProvider::ServantProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
}

Handler<Heaven> ServantProvider::heaven() const
{
	return heaven_.lock();
}

//---------------------------------------------------------

ServantObject::ServantObject(ServantProvider* provider)
:ReactiveNativeObject(provider)
{
}

void ServantObject::bootstrap(const Handler<Heap>& heap, Handler<Servant> const& angelTarget)
{
	this->ReactiveNativeObject::bootstrap(heap);
	this->servant_ = angelTarget;
}

Handler<World> ServantObject::world() const
{
	return servant_->world();
}

Handler<Servant> ServantObject::servant() const
{
	return this->servant_;
}


/**********************************************************************************************************************
 * BaseT
 **********************************************************************************************************************/
template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
ServantProviderBaseT<ProviderT, ObjectT, TargetT, AntiT>::ServantProviderBaseT(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ServantProvider(heap, provname, heaven)
{

}

//---------------------------------------------------------

template<typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
inline ServantObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::ServantObjectBaseT(ProviderT* provider)
:ServantObject(provider)
{
}

template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
Handler<TargetT> ServantObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::servant() const
{
	return Handler<TargetT>::__internal__fromRawPointerWithoutCheck( static_cast<TargetT*>(ServantObject::servant().get()) );
}


template <typename ProviderT, typename ObjectT, typename TargetT, typename AntiT>
void ServantObjectBaseT<ProviderT, ObjectT, TargetT, AntiT>::bootstrap(const Handler<Heap>& heap, const Handler<TargetT>& servant)
{
	this->ServantObject::bootstrap(heap, servant);
}

template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onFutureDiscarded(Handler<Heap> const& heap)
{

}
template <typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
void ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onHistoryDiscarded(Handler<Heap> const& heap)
{

}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiT newAnti;
	return Just(newAnti);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

template<typename ProviderT, typename ObjectT, typename AngelT, typename AntiT>
inline typename ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::ResultType ServantObjectBaseT<ProviderT, ObjectT, AngelT, AntiT>::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

/**********************************************************************************************************************
 * Element
 **********************************************************************************************************************/

ElementServantProvider::ElementServantProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "ElementServant", heaven)
{
	this->registerPureNativeClosure("element", [this](ElementServantObject* servant){
		return servant->servant()->element()->donutObject();
	});
	this->registerReactiveNativeClosure("enableAnimation", [this](ElementServantObject* servant_){
		auto servant = servant_->servant();
		bool enabled = servant->animEnabled();
		servant->animEnabled(true);
		AntiSideEffect anti;
		anti.op = enabled ? AntiSideEffect::None : AntiSideEffect::DisableAnimation;
		return std::tuple<bool,Maybe<AntiSideEffect> >(enabled, Just(anti));
	});
	this->registerReactiveNativeClosure("disableAnimation", [this](ElementServantObject* servant_){
		auto servant = servant_->servant();
		bool enabled = servant->animEnabled();
		servant->animEnabled(false);
		AntiSideEffect anti;
		anti.op = !enabled ? AntiSideEffect::None : AntiSideEffect::EnableAnimation;
		return std::tuple<bool,Maybe<AntiSideEffect> >(enabled, Just(anti));
	});
}

//---------------------------------------------------------

ElementServantObject::ElementServantObject(ElementServantProvider* provider)
:Super(provider)
{
}

void ElementServantObject::bootstrap(const Handler<Heap>& heap, const Handler<ElementServant>& servant)
{
	Super::bootstrap(heap, servant);
}

void ElementServantObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void ElementServantObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

typename ElementServantObject::ResultType ElementServantObject::execAntiSideEffect(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect anti;
	auto servant = this->servant();
	switch(val.op){
	case AntiSideEffect::EnableAnimation:
		anti.op = servant->animEnabled() ? AntiSideEffect::None : AntiSideEffect::DisableAnimation;
		servant->animEnabled(true);
		break;
	case AntiSideEffect::DisableAnimation:
		anti.op = !servant->animEnabled() ? AntiSideEffect::None : AntiSideEffect::EnableAnimation;
		servant->animEnabled(false);
		break;
	case AntiSideEffect::None:
		CINAMO_EXCEPTION(Exception, "[BUG] None operation is not handled.")
	}
	return Just<AntiSideEffect>(anti);
}
typename ElementServantObject::ResultType ElementServantObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	switch(val.op){
	case AntiSideEffect::None:
		return Super::onBack(heap, val);
	default:
		return execAntiSideEffect(heap, val);
	}
}

typename ElementServantObject::ResultType ElementServantObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	switch(val.op){
	case AntiSideEffect::None:
		return Super::onForward(heap, val);
	default:
		return execAntiSideEffect(heap, val);
	}
}

XValue ElementServantObject::saveImpl(const Handler<Heap>& heap)
{
}

void ElementServantObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::string ElementServantObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::cinamo::format("(ElementServantObject: %p)", this);
}


/**********************************************************************************************************************
 * Halo
 **********************************************************************************************************************/

HaloServantProvider::HaloServantProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "HaloServant", heaven)
{
}

//---------------------------------------------------------

HaloServantObject::HaloServantObject(HaloServantProvider* provider)
:Super(provider)
{
}

void HaloServantObject::bootstrap(const Handler<Heap>& heap, const Handler<HaloServant>& servant)
{
	Super::bootstrap(heap, servant);
}

void HaloServantObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void HaloServantObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

typename HaloServantObject::ResultType HaloServantObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

typename HaloServantObject::ResultType HaloServantObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

XValue HaloServantObject::saveImpl(const Handler<Heap>& heap)
{
}

void HaloServantObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::string HaloServantObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::cinamo::format("(HaloServantObject: %p)", this);
}

/**********************************************************************************************************************
 * ContentUnderline
 **********************************************************************************************************************/

ContentUnderlineServantProvider::ContentUnderlineServantProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "ContentUnderlineServant", heaven)
{
}

//---------------------------------------------------------

ContentUnderlineServantObject::ContentUnderlineServantObject(ContentUnderlineServantProvider* provider)
:Super(provider)
{
}

void ContentUnderlineServantObject::bootstrap(const Handler<Heap>& heap, const Handler<ContentUnderlineServant>& servant)
{
	Super::bootstrap(heap, servant);
}

void ContentUnderlineServantObject::onFutureDiscarded(const Handler<Heap>& heap)
{
	Super::onFutureDiscarded(heap);
}

void ContentUnderlineServantObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
	Super::onHistoryDiscarded(heap);
}

ContentUnderlineServantObject::ResultType ContentUnderlineServantObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onBack(heap, val);
}

ContentUnderlineServantObject::ResultType ContentUnderlineServantObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	return Super::onForward(heap, val);
}

XValue ContentUnderlineServantObject::saveImpl(const Handler<Heap>& heap)
{
}

void ContentUnderlineServantObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

std::string ContentUnderlineServantObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::cinamo::format("(ContentUnderlineServantObject: %p)", this);
}
}}


