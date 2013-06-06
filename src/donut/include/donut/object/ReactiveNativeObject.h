/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "ReactiveNativeObjectDecl.h"
#include "HeapDecl.h"

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
	int const newIndex = this->findUpperIndex(clock->now());
	this->index_ = newIndex;
	bool failed = false;
	for(int i=nowIndex-1; i>=newIndex;--i){
		std::pair<timestamp_t, AntiSideEffect>& p = reactions_[i];
		Maybe<AntiSideEffect> res ( self.onBack(heap, p.second) );
		if( res.isJust ) {
			p.second = res.value();
		}else{
			failed |= true;
		}
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
		Maybe<AntiSideEffect> res ( self.onForward(heap, p.second) );
		if( res.isJust ){
			p.second = res.value();
		}else{
			failed |= true;
		}
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
		self.setUseLock( !this->reactions_.empty() );
	}
	self.onHistoryDiscarded(heap);
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::onDiscardFutureNotify(Self& self, Handler<Heap> const& heap) {
	if(this->reactions_.size() > 0){
		this->index_ = findUpperIndex(heap->clock()->now());
		this->reactions_.erase(this->reactions_.begin()+this->index_, this->reactions_.end());
		self.setUseLock( !this->reactions_.empty() );
	}
	self.onFutureDiscarded(heap);
}

template <typename AntiSideEffect, typename Self>
void ReactiveNativeObjectAspectT<AntiSideEffect, Self>::registerReaction( Handler<HeapObject> const& obj, timestamp_t time, AntiSideEffect const& v ) {
	if( unlikely( this->reactions_.size() > 0 && this->reactions_.back().first > time ) ){
		DONUT_EXCEPTION(Exception, "[BUG] Reaction table was broken!!");
	}
	this->reactions_.push_back(std::pair<timestamp_t, AntiSideEffect>(time, v));
	obj->setUseLock( true );
	this->index_ = this->reactions_.size();
}
}
