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
#include "ObjectWalker.h"
#include "Slot.h"

namespace chisa {
namespace donut {

void DonutObject::walkImpl(ObjectWalker* walker)
{
	if(this->walkColor() == walker->color()){
		return;
	}
	this->walkColor(walker->color());
	walker->onWalk(this);
	for(auto slot : this->slots_){
		slot.second.walk(walker);
	}
}

void Slot::walk(ObjectWalker* walker)
{
	typedef decltype(this->rev_) ListType;
	for(const std::pair<unsigned int, Object*>& rev : this->rev_){
		if( rev.second ) {
			rev.second->walk(walker);
		}
	}
}

void NativeObject::walkImpl(ObjectWalker* walker)
{
	if(this->walkColor() == walker->color()){
		return;
	}
	this->walkColor(walker->color());
	walker->onWalk(this);
	this->prototype_->walk(walker);
}

void NativeClosureObject::walkImpl(ObjectWalker* walker)
{
	if(this->walkColor() == walker->color()){
		return;
	}
	this->walkColor(walker->color());
	walker->onWalk(this);
}

}}
