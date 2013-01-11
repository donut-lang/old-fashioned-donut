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

#pragma once
#include "ReactiveNativeObjectDecl.h"

namespace donut {


template <typename AntiSideEffect, typename Self>
XValue ReactiveNativeObjectAspectT<AntiSideEffect, Self>::save() {
	XArchiverOut out;
	out & reactions_;
	out & index_;
	return out.toXValue();
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::load( XValue const& data ) {
	XArchiverIn in(data);
	in & reactions_;
	in & index_;
}

template <typename AntiSideEffect, typename Self>
int inline ReactiveNativeObjectAspectT<AntiSideEffect, Self>::findUpperIndex( timestamp_t const& t ) {
	return std::distance(this->reactions_.begin(),
			std::upper_bound(this->reactions_.begin(), this->reactions_.end(), t, PairCompare<timestamp_t, AntiSideEffect>()));
}

template <typename AntiSideEffect, typename Self>
int inline ReactiveNativeObjectAspectT<AntiSideEffect, Self>::findLowerIndex( timestamp_t const& t ) {
	return std::distance(this->reactions_.begin(),
			std::lower_bound(this->reactions_.begin(), this->reactions_.end(), t, PairCompare<timestamp_t, AntiSideEffect>()));
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::onBackNotify(Self& self, Handler<Heap> const& heap) {
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findLowerIndex(clock->now());
	this->index_ = newIndex;
	bool failed = false;
	for(int i=nowIndex-1; i>=newIndex;--i){
		std::pair<timestamp_t, AntiSideEffect>& p = reactions_[i];
		bool f;
		std::tie(f, p.second) = self.onBack(heap, p.second);
		failed |= !f;
	}
	if( failed ){ //もうもどれない
		clock->discardFuture();
	}
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::onForwardNotify(Self& self, Handler<Heap> const& heap) {
	Handler<Clock> clock = heap->clock();
	int const nowIndex = this->index_;
	int const newIndex = this->findUpperIndex(clock->now());
	this->index_ = newIndex;
	auto start = reactions_.begin() + nowIndex;
	auto end = reactions_.begin() + newIndex;
	bool failed = false;
	for(auto it = start; it != end; ++it) {
		std::pair<timestamp_t, AntiSideEffect>& p = *it;
		bool f;
		std::tie(f, p.second) = self.onForward(heap, p.second);
		failed |= !f;
	}
	if( failed ){ //もうもどれない
		clock->discardHistory();
	}
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::onDiscardHistoryNotify(Self& self, Handler<Heap> const& heap) {
	if(this->reactions_.size() > 0) {
		this->reactions_.erase(this->reactions_.begin(), this->reactions_.begin()+findLowerIndex(heap->clock()->now()));
		this->index_ = 0;
	}
	self.onHistoryDiscarded(heap);
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::onDiscardFutureNotify(Self& self, Handler<Heap> const& heap) {
	if(this->reactions_.size() > 0){
		this->index_ = findUpperIndex(heap->clock()->now());
		this->reactions_.erase(this->reactions_.begin()+this->index_, this->reactions_.end());
	}
	self.onFutureDiscarded(heap);
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::registerReaction( timestamp_t time, AntiSideEffect const& v ) {
	if( unlikely( this->reactions_.size() > 0 && this->reactions_.back().first > time ) ){
		DONUT_EXCEPTION(Exception, "[BUG] Reaction table was broken!!");
	}
	this->reactions_.push_back(std::pair<timestamp_t, AntiSideEffect>(time, v));
	this->index_ = this->reactions_.size();
}
}