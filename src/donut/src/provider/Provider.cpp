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

#include <donut/provider/Provider.h>
#include <donut/object/Heap.h>
#include <donut/Exception.h>

namespace donut {

Provider::Provider( Handler<Heap> const& heap, std::string const& name )
:heap_(heap), name_(name)
{
}

PureNativeClosureObject::Signature const& Provider::findPureNativeClosureEntry( std::string const& name )
{
	auto it = pureNativeClosures_.find(name);
	if(it == this->pureNativeClosures_.end()){
		DONUT_EXCEPTION(Exception, "Pure Native Closure \"%s\" not found in \"%s\"!!", name.c_str(), this->name().c_str());
	}
	VectorMap<std::string, PureNativeClosureObject::Signature>::Pair const& p = *it;
	return p.second;
}
ReactiveNativeClosureObject::Signature const& Provider::findReactiveNativeClosureEntry( std::string const& name )
{
	auto it = reactiveNativeClosures_.find(name);
	if(it == this->reactiveNativeClosures_.end()){
		DONUT_EXCEPTION(Exception, "Reactive Native Closure \"%s\" not found in \"%s\"!!", name.c_str(), this->name().c_str());
	}
	VectorMap<std::string, ReactiveNativeClosureObject::Signature>::Pair const& p = *it;
	return p.second;
}

//関数ポインタ版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Function> ( std::string const& name, PureNativeClosureObject::Function f) {
	return (!pureNativeClosures_.have(name)) && pureNativeClosures_.update( name, f );
}

template <>
bool Provider::registerReactiveNativeClosure<ReactiveNativeClosureObject::Function> ( std::string const& name, ReactiveNativeClosureObject::Function f) {
	return (!reactiveNativeClosures_.have(name)) && reactiveNativeClosures_.update( name, f );
}

// std::function版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Signature>( std::string const& name, PureNativeClosureObject::Signature f) {
	return (!pureNativeClosures_.have(name)) && pureNativeClosures_.update( name, f );
}

template <>
bool Provider::registerReactiveNativeClosure<ReactiveNativeClosureObject::Signature>( std::string const& name, ReactiveNativeClosureObject::Signature f) {
	return (!reactiveNativeClosures_.have(name)) && reactiveNativeClosures_.update( name, f );
}

/******************************************************************************
 * save/load
 ******************************************************************************/

void Provider::bootstrap()
{
	if(Handler<Heap> heap = this->heap().lock()){
		this->prototype_ = heap->createEmptyDonutObject();
		for( std::pair<std::string, PureNativeClosureObject::Signature> const& p : this->pureNativeClosures_ ){
			this->prototype_->set(heap, p.first, heap->createPureNativeClosureObject(this->name(), p.first, p.second));
		}
		for( std::pair<std::string, ReactiveNativeClosureObject::Signature> const& p : this->reactiveNativeClosures_ ){
			this->prototype_->set(heap, p.first, heap->createReactiveNativeClosureObject(this->name(), p.first, p.second));
		}
	}
}
XValue Provider::save()
{
	Handler<XObject> xobj(new XObject);
	xobj->set("prototype", this->prototype_->toDescriptor());
	xobj->set("content", this->saveImpl());
	return xobj;
}

void Provider::load( XValue const& data)
{
	Handler<XObject> xobj(data.as<XObject>());
	if(Handler<Heap> heap = this->heap().lock()){
		this->prototype_ = heap->decodeHeapDescriptor(xobj->get<object_desc_t>("prototype")).cast<DonutObject>();
	}
	this->loadImpl(xobj->get<XValue>("content"));
}

/******************************************************************************
 * default implementation
 ******************************************************************************/
XValue Provider::saveImpl() {
	return XValue();
}
void Provider::loadImpl( XValue const& data){

}

}
