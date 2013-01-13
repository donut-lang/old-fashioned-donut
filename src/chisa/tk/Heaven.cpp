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

#include "Heaven.h"
#include "Angel.h"
#include "World.h"
#include "heaven/LoneAngel.h"
#include "heaven/TwinAngel.h"
#include "donut/Patron.h"
#include "donut/WorldObject.h"

namespace chisa {
namespace tk {

Heaven::Heaven(const Handler<World>& world)
:world_(world)
{
}


Handler< ::donut::Object> Heaven::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(!this->donutObject_.expired()){
		return this->donutObject_.lock();
	}
	Handler<World> world(this->world().lock());
	if(!world) {
		return Handler< ::donut::Object>();
	}
	Handler<HeavenObject> obj( world->patron()->heavenProvider()->newInstance(heap, self()) );
	this->donutObject_ = obj;
	return obj;
}

void Heaven::render(gl::Canvas& canvas)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->render(canvas);
	}
}

void Heaven::idle(const float delta_ms)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->idle(delta_ms);
	}
}

void Heaven::reshape(const geom::Area& area)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		angel->reshape(area);
	}
}

Handler<Angel> Heaven::findAngelById(const std::string& id)
{
	VectorMap<std::string, Handler<Angel> >::Iterator it = this->angelMap_.find(id);
	VectorMap<std::string, Handler<Angel> >::Pair& p = *it;
	if( unlikely(it == angelMap_.end()) ) {
		return Handler<Angel>();
	}
	return p.second;
}

std::string Heaven::findAngelId(const Handler<Angel>& angel)
{
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
		if(p.second == angel){
			return p.first;
		}
	}
	return "";
}

void Heaven::attatchAngel(const std::string& id, const Handler<Angel>& angel)
{
	this->angelMap_.update(id, angel);
	angel->reshape( this->world().lock()->area() );
}

std::string Heaven::attatchAngel(const Handler<Angel>& angel)
{
	do{
		std::string const id( ::tarte::randomString(10));
		if( this->angelMap_.insert(id, angel) ) {
			angel->reshape( this->world().lock()->area() );
			return id;
		}
	} while(true);
	return "";
}

Handler<Angel> Heaven::newTwinAngel()
{
	return Handler<Angel>( new TwinAngel(self()) );
}

Handler<Angel> Heaven::detatchAngel(const Handler<Angel>& angel)
{
	for( auto it = this->angelMap_.begin(); it != this->angelMap_.end(); ++it ) {
		VectorMap<std::string, Handler<Angel> >::Pair& p = *it;
		Handler<Angel> const& angel = p.second;
		if(p.second == angel){
			this->angelMap_.erase(it);
			return angel;
		}
	}
	return Handler<Angel>();
}

Handler<Angel> Heaven::detatchAngel(const std::string& id)
{
	VectorMap<std::string, Handler<Angel> >::Iterator it = this->angelMap_.find(id);
	VectorMap<std::string, Handler<Angel> >::Pair& p = *it;
	if( unlikely(it == angelMap_.end()) ) {
		return Handler<Angel>();
	}
	Handler<Angel> angel(p.second);
	this->angelMap_.erase(it);
	return angel;
}

Handler<Element> Heaven::findElementByPoint(geom::Point const& screenVector)
{
	Handler<Element> elm;
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
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
	for(VectorMap<std::string, Handler<Angel> >::Pair& p : this->angelMap_ ) {
		Handler<Angel> const& angel = p.second;
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
