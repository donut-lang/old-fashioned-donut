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

#include "Object.h"
#include "Slot.h"
#include "../Clock.h"

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
