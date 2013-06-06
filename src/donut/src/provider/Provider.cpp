/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/Exception.h>
#include <donut/object/Heap.h>
#include <donut/provider/Provider.h>

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

//関数ポインタ版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Function> ( std::string const& name, PureNativeClosureObject::Function f) {
	return (!pureNativeClosures_.have(name)) && pureNativeClosures_.update( name, f );
}

// std::function版
template <>
bool Provider::registerPureNativeClosure<PureNativeClosureObject::Signature>( std::string const& name, PureNativeClosureObject::Signature f) {
	return (!pureNativeClosures_.have(name)) && pureNativeClosures_.update( name, f );
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
