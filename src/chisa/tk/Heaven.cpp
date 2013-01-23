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
	if(this->donutObject_.expired()){
		Handler<World> world(this->world().lock());
		if( unlikely(!world) ){
			TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
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
