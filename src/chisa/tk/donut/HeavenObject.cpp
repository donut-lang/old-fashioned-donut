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
#include "HeavenObject.h"
#include "ElementObject.h"
#include "../Heaven.h"
#include "../Angel.h"
#include "AngelObject.h"

namespace chisa {
namespace tk {

const static std::string TAG("HeavenObject");

HeavenProvider::HeavenProvider(const Handler<Heap>& heap, const Handler<Heaven>& heaven)
:Super(heap, "chisa::tk::HeavenObject")
{
	this->registerPureNativeClosure("newTwinAngel", [this](HeavenObject* obj){
		return obj->heaven()->newTwinAngel()->donutObject(this->heap().lock());
	});
	this->registerPureNativeClosure("newLoneAngel", [this](HeavenObject* obj){
		return obj->heaven()->newLoneAngel()->donutObject(this->heap().lock());
	});
	this->registerReactiveNativeClosure("attatchAngel", [this](HeavenObject* obj, AngelObject* angel)->std::tuple<bool,bool,HeavenSideEffect>{
		bool succeed = obj->heaven()->attatchAngel(angel->angel());

		HeavenSideEffect side;
		side.op = HeavenSideEffect::DetatchAngel;
		side.detatchedAngel_ = angel->angel();
		return std::tuple<bool,bool,HeavenSideEffect>(succeed, true, side);
	});
	this->registerReactiveNativeClosure("detatchAngel", [this](HeavenObject* obj, AngelObject* angel)->std::tuple<AngelObject* ,bool,HeavenSideEffect>{
		obj->heaven()->detatchAngel(angel->angel());

		HeavenSideEffect side;
		side.op = HeavenSideEffect::AttatchAngel;
		side.attatchedAngel_ = angel->angel();
		return std::make_tuple(angel,true,side);
	});
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

HeavenObject::HeavenObject(HeavenProvider* provider)
:Super(provider)
{
}

void HeavenObject::bootstrap(const Handler<Heap>& heap, Handler<Heaven> const& heaven)
{
	this->Super::bootstrap(heap);
	this->heaven_ = heaven;
}

Handler<Heaven> HeavenObject::heaven() const
{
	return this->heaven_.lock();
}

std::string HeavenObject::reprImpl(const Handler<Heap>& heap) const
{
	return ::cinamo::format("(HeavenObject: %p)", this);
}

HeavenObject::ResultType HeavenObject::execAntiSideEffect(Handler<Heap> const& heap, AntiSideEffect const& val)
{
	HeavenSideEffect side;
	switch (val.op){
	case HeavenSideEffect::DetatchAngel:
		this->heaven_.lock()->detatchAngel( val.detatchedAngel_ );
		side.op = HeavenSideEffect::AttatchAngel;
		side.attatchedAngel_ = val.detatchedAngel_;
		break;
	case HeavenSideEffect::AttatchAngel:
		this->heaven_.lock()->attatchAngel(val.attatchedAngel_);
		side.op = HeavenSideEffect::DetatchAngel;
		side.detatchedAngel_ = val.attatchedAngel_;
		break;
	}
	return ResultType(true, side);
}

HeavenObject::ResultType HeavenObject::onBack(const Handler<Heap>& heap, const HeavenSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

HeavenObject::ResultType HeavenObject::onForward(const Handler<Heap>& heap, const HeavenSideEffect& val)
{
	return this->execAntiSideEffect(heap, val);
}

XValue HeavenObject::saveImpl(const Handler<Heap>& heap)
{
	//XXX
	return XValue();
}

void HeavenObject::onFutureDiscarded(const Handler<Heap>& heap)
{
}

void HeavenObject::onHistoryDiscarded(const Handler<Heap>& heap)
{
}

void HeavenObject::loadImpl(const Handler<Heap>& heap, const XValue& data)
{
	//XXX
}

}}
