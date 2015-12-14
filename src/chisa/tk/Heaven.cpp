/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Heaven.hpp"
#include "Angel.hpp"
#include "World.hpp"
#include "heaven/LoneAngel.hpp"
#include "heaven/TwinAngel.hpp"
#include "donut/Patron.hpp"
#include "donut/WorldObject.hpp"

namespace chisa {
namespace tk {

Heaven::Heaven(const Handler<World>& world)
:world_(world)
{
}


Handler< ::donut::Object> Heaven::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(this->donutObject_.expired()){
		Handler<World> world(this->world().lock());
		if( unlikely(!world) ){
			CINAMO_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
		}
		Handler< ::donut::Object> obj(world->patron()->heavenProvider()->newInstance(heap, self()));
		this->donutObject_ = obj;
		return obj;
	}
	return this->donutObject_.lock();
}

void Heaven::render(gl::Canvas& canvas)
{
	for( Handler<Angel> const& angel : this->angels_ ) {
		angel->render(canvas);
	}
}

void Heaven::idle(const float delta_ms)
{
	for( Handler<Angel> const& angel : this->angels_ ) {
		angel->idle(delta_ms);
	}
}

void Heaven::reshape(const geom::Area& area)
{
	for( Handler<Angel> const& angel : this->angels_ ) {
		angel->reshape(area);
	}
}

bool Heaven::attatchAngel(const Handler<Angel>& angel)
{
	auto it = std::find(angels_.begin(), angels_.end(), angel);
	if( it != this->angels_.end() ) {
		return false;
	}
	this->angels_.push_back(angel);
	angel->onAttatched();
	angel->onShown();
	angel->reshape( this->world().lock()->area() );
	return true;
}

Handler<Angel> Heaven::newTwinAngel()
{
	return Handler<Angel>( new TwinAngel(self()) );
}

Handler<Angel> Heaven::detatchAngel(const Handler<Angel>& angel)
{
	auto it = std::find(angels_.begin(), angels_.end(), angel);
	if( it == this->angels_.end() ) {
		return Handler<Angel>();
	}
	this->angels_.erase(it);
	angel->onHidden();
	angel->onDetatched();
	return angel;
}

Handler<Element> Heaven::findElementByPoint(geom::Point const& screenVector)
{
	Handler<Element> elm;
	for( auto it = angels_.crbegin(); it != angels_.crend(); ++it ){
		Handler<Angel> const& angel = *it;
		elm = angel->findElementByPoint(screenVector);
		if( elm ){
			break;
		}
	}
	return elm;
}

Handler<Element> Heaven::findElementById(const std::string& id)
{
	Handler<Element> elm;
	for( Handler<Angel> const& angel : this->angels_ ) {
		elm = angel->findElementById(id);
		if( elm ){
			break;
		}
	}
	return elm;
}

Handler<Angel> Heaven::newLoneAngel()
{
	return Handler<Angel>( new LoneAngel(self()) );
}

}}
