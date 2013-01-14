/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "NesGeistObject.h"

namespace nes {
using namespace tarte;

NesGeistProvider::NesGeistProvider(Handler< ::donut::Heap> const& heap)
:Super(heap)
{
	this->registerReactiveNativeClosure("writeNES",[](NesGeistObject* obj, uint16_t addr, uint16_t val){
		Handler<NesGeist> geist(obj->geist());
		NesGeist::MachineLock lock(geist);
		NesGeistSideEffect side;
		side.op = NesGeistSideEffect::LoadSave;
		side.save = geist->machine()->save();
		geist->machine()->write(addr, val);

		return std::make_tuple(val,true,side);
	});
}

//---------------------------------------------------------------------------------------

NesGeistObject::NesGeistObject(NesGeistProvider* provider)
:Super(provider)
{
}

void NesGeistObject::onFutureDiscarded(const Handler< ::donut::Heap>& heap)
{
}

void NesGeistObject::onHistoryDiscarded(const Handler< ::donut::Heap>& heap)
{
}

typename NesGeistObject::ResultType NesGeistObject::execAntiSideEffect(Handler< ::donut::Heap> const& heap, AntiSideEffect const& val)
{
	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	AntiSideEffect anti;
	switch(val.op){
	case AntiSideEffect::None:
		break;
	case AntiSideEffect::LoadSave:
		anti.op = AntiSideEffect::LoadSave;
		anti.save = geist->machine()->save();
		geist->machine()->load(val.save);
		break;
	}
	return std::tuple<bool, AntiSideEffect>(true, anti);
}

typename NesGeistObject::ResultType NesGeistObject::onBack(const Handler< ::donut::Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onBack(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

typename NesGeistObject::ResultType NesGeistObject::onForward(const Handler< ::donut::Heap>& heap, const AntiSideEffect& val)
{
	if(val.op == AntiSideEffect::None) {
		return Super::onForward(heap,val);
	}
	return execAntiSideEffect(heap,val);
}

XValue NesGeistObject::saveImpl(const Handler< ::donut::Heap>& heap)
{
}

void NesGeistObject::loadImpl(const Handler< ::donut::Heap>& heap, const XValue& data)
{
}


}
