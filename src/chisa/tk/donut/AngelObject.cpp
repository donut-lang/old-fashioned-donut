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

namespace chisa {
namespace tk {

const static std::string TAG("AngelObject");

AngelProvider::AngelProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
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

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LoneAngelProvider::LoneAngelProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:AngelProvider(heap, "LoneAngelProvider", heaven)
{
}

LoneAngelObject::LoneAngelObject(AngelProvider* provider)
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
}

LoneAngelObject::ResultType LoneAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
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

TwinAngelObject::TwinAngelObject(AngelProvider* provider)
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
}

TwinAngelObject::ResultType TwinAngelObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

XValue TwinAngelObject::saveImpl(const Handler<Heap>& heap)
{
}

void TwinAngelObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

}}


