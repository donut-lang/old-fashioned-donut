/**
 * Donut
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

#include <tarte/PairFunctor.h>
#include <tarte/String.h>
#include <algorithm>

#include <donut/object/ReactiveNativeObject.h>
#include <donut/object/Heap.h>
#include <donut/Clock.h>

namespace donut {

const std::string TAG("ReactiveNativeObject");

std::string ReactiveNativeObject::reprImpl(Handler<Heap> const& heap) const
{
	return format("(ReactiveNativeObject: %p)", this);
}

int ReactiveNativeObject::findLowerIndex( timestamp_t const& t )
{
	auto it = std::lower_bound(this->reactions_.begin(), this->reactions_.end(), t, PairCompare<timestamp_t, XValue>());
	return std::distance(this->reactions_.begin(), it);
}
int ReactiveNativeObject::findUpperIndex( timestamp_t const& t )
{
	auto it = std::upper_bound(this->reactions_.begin(), this->reactions_.end(), t, PairCompare<timestamp_t, XValue>());
	return std::distance(this->reactions_.begin(), it);
}

void ReactiveNativeObject::onBackNotifyImpl(Handler<Heap> const& heap)
{
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findLowerIndex(clock->now());
	this->index_ = newIndex;
	bool failed = false;
	for(int i=nowIndex-1; i>=newIndex;--i){
		std::pair<timestamp_t, XValue>& p = reactions_[i];
		XValue val ( this->onBack(heap, p.second) );
		failed |= (val.is<XNull>());
		p.second = val;
	}
	if( failed ){ //もうもどれない
		clock->discardFuture();
	}
}

void ReactiveNativeObject::onForwardNotifyImpl(Handler<Heap> const& heap)
{
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findUpperIndex(clock->now());
	this->index_ = newIndex;
	auto start = reactions_.begin() + nowIndex;
	auto end = reactions_.begin() + newIndex;
	bool failed = false;
	for(auto it = start; it != end; ++it) {
		std::pair<timestamp_t, XValue>& p = *it;
		XValue val ( this->onForward(heap, p.second) );
		failed |= (val.is<XNull>());
		p.second = val;
	}
	if( failed ){ //もうもどれない
		clock->discardHistory();
	}
}

void ReactiveNativeObject::onDiscardHistoryNotifyImpl(Handler<Heap> const& heap)
{
	if(this->reactions_.size() > 0){
		this->reactions_.erase(this->reactions_.begin(), this->reactions_.begin()+findLowerIndex(heap->clock()->now()));
		this->index_ = 0;
	}
	this->onHistoryDiscarded(heap);
}

void ReactiveNativeObject::onDiscardFutureNotifyImpl(Handler<Heap> const& heap)
{
	if(this->reactions_.size() > 0){
		this->index_ = findUpperIndex(heap->clock()->now());
		this->reactions_.erase(this->reactions_.begin()+this->index_, this->reactions_.end());
	}
	this->onFutureDiscarded(heap);
}

void ReactiveNativeObject::bootstrap( Handler<Heap> const& heap )
{
	this->NativeObject::bootstrap(heap);
}


void ReactiveNativeObject::registerReaction( timestamp_t time, XValue const& v )
{
	if( this->reactions_.size() > 0 && this->reactions_.back().first > time ){
		DONUT_EXCEPTION(Exception, "[BUG] Reaction table was broken!!");
	}
	this->reactions_.push_back(std::pair<timestamp_t, XValue>(time, v));
	this->index_ = this->reactions_.size();
}

/**********************************************************************************
 * save/load
 **********************************************************************************/

XValue ReactiveNativeObject::save( Handler<Heap> const& heap )
{
	Handler<XObject> top(new XObject);
	top->set("base", this->NativeObject::save(heap));
	return top;
}
void ReactiveNativeObject::load( Handler<Heap> const& heap, XValue const& data )
{
	Handler<XObject> top(data.as<XObject>());
	this->NativeObject::load(heap, top->get<XValue>("base"));
}

}
