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

#include "Provider.h"
#include "../object/NativeObject.h"
#include "../object/Heap.h"
#include "../Exception.h"
#include <tinyxml2.h>

namespace chisa {
namespace donut {

Provider::Provider( const Handler<Heap>& heap, const std::string& name )
:heap_(heap), name_(name)
{
}

void Provider::addPrototype( const std::string& name, Handler<NativeClosureEntry> clos )
{
	if(Handler<Heap> heap = this->heap().lock()){
		this->prototype_->set(heap, name, clos->createObject(heap, this->name(), name));
	}
}

void Provider::bootstrap()
{
	if(Handler<Heap> heap = this->heap().lock()){
		this->prototype_ = heap->createEmptyDonutObject();
		for( std::pair<std::string, Handler<NativeClosureEntry>> const& p : this->nativeClosures_ ){
			this->prototype_->set(heap, p.first, p.second->createObject(heap, this->name(), p.first));
		}
	}
}
util::XValue Provider::save()
{

}

void Provider::load( util::XValue const& data)
{

}

Handler<NativeClosureEntry> const& Provider::findClosureEntry( std::string const& name )
{
	auto it = nativeClosures_.find(name);
	if(it == this->nativeClosures_.end()){
		throw DonutException(__FILE__, __LINE__, "Closure %s not found in %s!!", name.c_str(), this->name().c_str());
	}
	util::VectorMap<std::string, Handler<NativeClosureEntry> >::Pair const& p = *it;
	return p.second;
}
}}
