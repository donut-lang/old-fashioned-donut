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

#include "NativeObject.h"
#include <algorithm>
#include "Heap.h"
#include "../Clock.h"

namespace chisa {
namespace donut {

const std::string TAG("ReactiveNativeObject");

std::string ReactiveNativeObject::toStringImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to String implicitly. Use toString()");
}

int ReactiveNativeObject::toIntImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Integer implicitly. Use toInteger()");
}

float ReactiveNativeObject::toFloatImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Float implicitly. Use toFloat()");
}

bool ReactiveNativeObject::toBoolImpl(Handler<Heap> const& heap) const
{
	throw DonutException(__FILE__, __LINE__, "[BUG] You cannot cast Reactive Native Object to Bool implicitly. Use toBoolean()");
}

int ReactiveNativeObject::findIndex( timestamp_t const& t )
{
	auto it = std::lower_bound(this->reactions_.begin(), this->reactions_.end(), t, util::PairCompare<timestamp_t, util::XValue>());
	return std::distance(this->reactions_.begin(), it);
}

void ReactiveNativeObject::onBackNotifyImpl(Handler<Heap> const& heap)
{
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findIndex(clock->now());
	this->index_ = newIndex;
	auto start = reactions_.begin() + nowIndex;
	auto end = reactions_.begin() + newIndex;
	bool failed = false;
	for(auto it = start; it != end; --it) {
		std::pair<timestamp_t, util::XValue>& p = *it;
		bool discard;
		util::XValue val;
		std::tie(discard, val) = this->onBack(heap, p.second);
		failed |= !discard;
		p.second = val;
	}
	if( failed ){ //もうもどれない
		clock->discardHistory();
	}
}

void ReactiveNativeObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findIndex(clock->now());
	this->index_ = newIndex;
	auto start = reactions_.begin() + nowIndex;
	auto end = reactions_.begin() + newIndex;
	bool failed = false;
	for(auto it = start; it != end; ++it) {
		std::pair<timestamp_t, util::XValue>& p = *it;
		bool discard;
		util::XValue val;
		std::tie(discard, val) = this->onForward(heap, p.second);
		failed |= !discard;
		p.second = val;
	}
	if( failed ){ //もうもどれない
		clock->discardFuture();
	}
}

void ReactiveNativeObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{
	this->reactions_.erase(this->reactions_.begin(), this->reactions_.begin()+this->index_);
	this->index_ = 0;
	this->onHistoryDiscarded(heap);
}

void ReactiveNativeObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{
	this->reactions_.erase(this->reactions_.begin()+this->index_+1, this->reactions_.end());
	this->onFutureDiscarded(heap);
}

void ReactiveNativeObject::bootstrap( Handler<Heap> const& heap )
{
	this->NativeObject::bootstrap(heap);
}


void ReactiveNativeObject::registerReaction( timestamp_t time, util::XValue const& v )
{
	if( this->reactions_.size() > 0 && this->reactions_.back().first > time ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Reaction table was broken!!");
	}
	this->reactions_.push_back(std::pair<timestamp_t, util::XValue>(time, v));
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

util::XValue ReactiveNativeObject::save( Handler<Heap> const& heap )
{
	using namespace chisa::util;
	Handler<XObject> top(new XObject);
	top->set("base", this->NativeObject::save(heap));
	return top;
}
void ReactiveNativeObject::load( Handler<Heap> const& heap, util::XValue const& data )
{
	using namespace chisa::util;
	Handler<XObject> top(data.as<XObject>());
	this->NativeObject::load(heap, top->get<XValue>("base"));
}

}}

