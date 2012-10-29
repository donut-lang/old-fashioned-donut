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

#include "Universe.h"
#include "World.h"
#include "../util/FileUtil.h"

static const std::string TAG("Universe");

namespace chisa {
namespace tk {

Universe::Universe(logging::Logger& log, Hexe* hexe)
:log_(log)
,hexe_(hexe)
,canvas_(log)
{
}
Universe::~Universe() {
}

void Universe::init(weak_ptr<Universe> _self)
{
	this->self_=_self;
}

void Universe::render()
{
	//TODO: 下のスタックについて、オフスクリーンレンダリングしたほうがいい？？
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->render(this->canvas_);
	}
}
void Universe::idle(const float delta_ms)
{
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->idle(delta_ms);
	}
}
void Universe::reshape(const geom::Area& area)
{
	if(log().t()){
		log().t(TAG, "reshaped: %s", area.toString().c_str());
	}
	this->canvas_.resize2d(area.box());
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->reshape(geom::Area(0,0, area.width(), area.height()));
	}
	this->area(area);
}

void Universe::createNewWorld(const string& worldName)
{
	shared_ptr<World> newWorld(World::create(log(),this->self_, worldName));
	this->worldStack.push(newWorld);
}

void Universe::notifyWorldEnd(weak_ptr<World> me)
{
	if(shared_ptr<World> world = me.lock()){
		const int idx = this->worldStack.indexOf(world);
		if(idx < 0){
			log().w(TAG, "oops. notified unknown world.");
			return;
		}
		this->worldStack.erase(idx);

		if(shared_ptr<World> topWorld = this->worldStack.top()){
			// FIXME: 下の画面について、以前よりサイズが変わってるようならreshape
			topWorld->reshape(this->area());
		}
	}else{
		log().w(TAG, "notified world end, but world was already dead.");
	}
}
void Universe::onTouchDown(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->onTouchDown(timeMs, pointerIndex, screenPoint);
	}
}

void Universe::onTouchUp(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->onTouchUp(timeMs, pointerIndex, screenPoint);
	}
}

void Universe::onTouchMove(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint)
{
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->onTouchMove(timeMs, pointerIndex, screenPoint);
	}
}
std::shared_ptr<chisa::WorldGeist> Universe::invokeWorldGeist(std::weak_ptr<tk::World> world, const std::string& nameOfGeist)
{
	if(log().t()){
		log().t(TAG, "Invoking: %s",nameOfGeist.c_str());
	}
	return this->hexe()->invokeWorldGeist(world, nameOfGeist);
}

Handler<gl::RawSprite> Universe::queryImage(const std::string& abs_filename)
{
	return this->canvas_.queryImage(abs_filename);
}

Handler<gl::RawSprite> Universe::queryRawSprite(const int width, const int height)
{
	return this->canvas_.queryRawSprite(width, height);
}


}}
