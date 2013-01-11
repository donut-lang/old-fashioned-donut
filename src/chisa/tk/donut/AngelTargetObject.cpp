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

namespace chisa {
namespace tk {

const static std::string TAG("AngelTargetObject");

AngelTargetProvider::AngelTargetProvider(Handler<Heap> const& heap, std::string const& provname, Handler<Heaven> const& heaven)
:ReactiveProvider(heap, provname)
,heaven_(heaven)
{
}

Handler<Heaven> AngelTargetProvider::heaven() const
{
	return heaven_.lock();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

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

std::string AngelTargetObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::tarte::format("(AngelTargetObject: %p)", this);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelElementTargetProvider::AngelElementTargetProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:AngelTargetProvider(heap, "AngelElementTarget", heaven)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelElementTargetObject::AngelElementTargetObject(AngelTargetProvider* provider)
:AngelTargetObject(provider)
{
}

void AngelElementTargetObject::bootstrap(const Handler<Heap>& heap, const Handler<AngelElementTarget>& AngelTarget)
{
}

Handler<World> AngelElementTargetObject::world() const
{
}

Handler<AngelElementTarget> AngelElementTargetObject::servant() const
{
}

void AngelElementTargetObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void AngelElementTargetObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

AngelElementTargetObject::ResultType AngelElementTargetObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

AngelElementTargetObject::ResultType AngelElementTargetObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

XValue AngelElementTargetObject::saveImpl(const Handler<Heap>& heap)
{
}

void AngelElementTargetObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelWidgetTargetProvider::AngelWidgetTargetProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:AngelTargetProvider(heap, "AngelWidgetTarget", heaven)
{
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AngelWidgetTargetObject::AngelWidgetTargetObject(AngelTargetProvider* provider)
:AngelTargetObject(provider)
{
}

void AngelWidgetTargetObject::bootstrap(const Handler<Heap>& heap, const Handler<AngelWidgetTarget>& AngelTarget)
{
}

Handler<World> AngelWidgetTargetObject::world() const
{
}

Handler<AngelWidgetTarget> AngelWidgetTargetObject::angelTarget() const
{
}

void AngelWidgetTargetObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void AngelWidgetTargetObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

AngelWidgetTargetObject::ResultType AngelWidgetTargetObject::onBack(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

AngelWidgetTargetObject::ResultType AngelWidgetTargetObject::onForward(const Handler<Heap>& heap, const AntiSideEffect& val)
{
}

XValue AngelWidgetTargetObject::saveImpl(const Handler<Heap>& heap)
{
}

void AngelWidgetTargetObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
}

}}


