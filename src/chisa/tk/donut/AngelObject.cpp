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

namespace chisa {
namespace tk {

const static std::string TAG("AngelObject");

AngelProvider::AngelProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
	this->registerPureNativeClosure("newElementTarget", [this](AngelObject* obj, std::string elementId){
		return obj->angel()->newElementTarget(elementId)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("newWidgetTarget", [this](AngelObject* obj, std::string elementId, std::string widgetGuide){
		return obj->angel()->newWidgetTarget(elementId, widgetGuide)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("findElementTarget", [this](AngelObject* obj, std::string elementId){
		return obj->angel()->findElementTarget(elementId)->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("findWidgetTarget", [this](AngelObject* obj, std::string elementId, std::string widgetGuide){
		return obj->angel()->findWidgetTarget(elementId, widgetGuide)->donutObject(this->heap().lock());
	});
}

Handler<Heaven> AngelProvider::heaven() const
{
	return heaven_.lock();
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelObject::AngelObject(AngelProvider* provider)
:ReactiveNativeObject(provider)
{
}

void AngelObject::bootstrap(const Handler<Heap>& heap, Handler<Angel> const& angel)
{
	this->ReactiveNativeObject::bootstrap(heap);
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

std::string AngelObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(AngelObject: %p)", this);
}

template <typename T>
T& AngelObject::execAntiSideEffect(Handler<Heap> const& heap, AngelSideEffect const& old, T& val)
{
	AngelSideEffect& side = val;
	switch(old.op){
	case AngelSideEffect::AttatchTarget:
		this->angel()->attatchTarget(old.attatchedTarget_);
		side.op = AngelSideEffect::DetatchTarget;
		side.detatchedTarget_ = old.attatchedTarget_;
		break;
	case AngelSideEffect::DetatchTarget:
		this->angel()->detatchTarget(old.attatchedTarget_);
		side.op = AngelSideEffect::AttatchTarget;
		side.attatchedTarget_ = old.detatchedTarget_;
		break;
	}
	return val;
}

template <typename T>
T& AngelObject::onBack(Handler<Heap> const& heap, AngelSideEffect const& old, T& val)
{
	return execAntiSideEffect(heap, old, val);
}

template <typename T>
T& AngelObject::onForward(Handler<Heap> const& heap, AngelSideEffect const& old, T& val)
{
	return execAntiSideEffect(heap, old, val);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LoneAngelProvider::LoneAngelProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:AngelProvider(heap, "LoneAngelProvider", heaven)
{
}

LoneAngelObject::LoneAngelObject(LoneAngelProvider* provider)
:AngelObject(provider)
{
}

void LoneAngelObject::bootstrap(const Handler<Heap>& heap, const Handler<LoneAngel>& angel)
{
}

Handler<LoneAngel> LoneAngelObject::angel() const
{
	return Handler<LoneAngel>::__internal__fromRawPointerWithoutCheck( static_cast<LoneAngel*>(AngelObject::angel().get()) );
}

void LoneAngelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void LoneAngelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

LoneAngelObject::ResultType LoneAngelObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect side;
	return LoneAngelObject::ResultType(true, this->AngelObject::onBack(heap, val, side));
}

LoneAngelObject::ResultType LoneAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect side;
	return LoneAngelObject::ResultType(true, this->AngelObject::onForward(heap, val, side));
}

XValue LoneAngelObject::saveImpl(const Handler<Heap>& heap)
{
}

void LoneAngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TwinAngelProvider::TwinAngelProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:AngelProvider(heap, "TwinAngelProvider", heaven)
{
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TwinAngelObject::TwinAngelObject(TwinAngelProvider* provider)
:AngelObject(provider)
{
}

void TwinAngelObject::bootstrap(const Handler<Heap>& heap, const Handler<TwinAngel>& angel)
{
}

Handler<TwinAngel> TwinAngelObject::angel() const
{
	return Handler<TwinAngel>::__internal__fromRawPointerWithoutCheck( static_cast<TwinAngel*>(AngelObject::angel().get()) );
}

void TwinAngelObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void TwinAngelObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

TwinAngelObject::ResultType TwinAngelObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect side;
	return TwinAngelObject::ResultType(true, this->AngelObject::onBack(heap, val, side));
}

TwinAngelObject::ResultType TwinAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
	AntiSideEffect side;
	return TwinAngelObject::ResultType(true, this->AngelObject::onForward(heap, val, side));
}

XValue TwinAngelObject::saveImpl(const Handler<Heap>& heap)
{
}

void TwinAngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

}}


