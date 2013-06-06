/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <functional>
#include "ElementObject.h"
#include "WorldObject.h"

namespace donut {
namespace native {

template <>
Handler<Object> encode< ::cinamo::Handler<chisa::tk::Element> >(Handler<Heap> const& heap, ::cinamo::Handler<chisa::tk::Element> val)
{
	return val ? Handler<Object>(val->donutObject()) : heap->createNull();
}

}}

namespace chisa {
namespace tk {

const static std::string TAG("ElementObject");

/**********************************************************************************************************************
 * Base
 **********************************************************************************************************************/

ElementProvider::ElementProvider( Handler<Heap> const& heap, std::string const& name, Handler<World> const& world )
:ReactiveProvider(heap, name)
,world_(world)
{
	this->registerPureNativeClosure("findElementById", &ElementObject::findElementById);
	this->registerPureNativeClosure("findRootElement", &ElementObject::findRootElement);
	this->registerPureNativeClosure("isEnabled", &ElementObject::isEnabled);
}

Handler<World> ElementProvider::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

//---------------------------------------------------------

ElementObject::ElementObject(ElementProvider* provider)
:ReactiveNativeObject(provider)
,world_(provider->world())
{
}

Handler<World> ElementObject::world() const
{
	Handler<World> w( world_.lock() );
	if(!w){
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	return w;
}

Handler<Element> ElementObject::findRootElement()
{
	return this->element()->findRootElement();
}

Handler<Object> ElementObject::findElementById(std::string const& id)
{
	Handler<Element> found (this->element()->findElementById(id));
	if( found ) {
		return found->donutObject();
	}else{
		return this->provider()->heap().lock()->createNull();
	}
}

bool ElementObject::isEnabled()
{
	return element()->enabled();
}

}}
