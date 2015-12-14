/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/Clock.hpp>
#include <donut/object/Object.hpp>
#include <donut/object/Slot.hpp>

namespace donut {

void DonutObject::markImpl(Handler<Heap> const& heap, int color)
{
	if(this->color() == color){
		return;
	}
	this->color(color);
	for(auto slot : this->slots_){
		slot.second.mark(heap, color);
	}
}

void Slot::mark(Handler<Heap> const& heap, int color)
{
	typedef decltype(this->rev_) ListType;
	for(const std::pair<unsigned int, Object*>& rev : this->rev_){
		if( rev.second ) {
			rev.second->mark(heap, color);
		}
	}
}

void NativeObject::markImpl(Handler<Heap> const& heap, int color)
{
	if(this->color() == color){
		return;
	}
	this->color(color);
	this->prototype_->mark(heap, color);
}

void NativeClosureObject::markImpl(Handler<Heap> const& heap, int color)
{
	if(this->color() == color){
		return;
	}
	this->color(color);
}

}
