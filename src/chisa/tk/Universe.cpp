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
#include "../util/FileUtil.h"

static const std::string TAG("Universe");

namespace chisa {
namespace tk {

Universe::Universe(logging::Logger& log, const std::string& basepath)
:log(log)
,basepath_(basepath)
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
void Universe::reshape(const Area& area)
{
	if(log.t()){
		log.t(TAG, "reshaped: %s", area.toString().c_str());
	}
	this->canvas_.resize2d(area.width(), area.height());
	if(shared_ptr<World> topWorld = this->worldStack.top()){
		topWorld->reshape(Area(0,0, area.width(), area.height()));
	}
	this->area(area);
}

void Universe::createNewWorld(const string& worldName)
{
	const string worldBasePath = this->basepath_+util::FileUtil::Sep+worldName;
	shared_ptr<World> newWorld(World::create(log,this->self_, worldName));
	this->worldStack.push(newWorld);
}

void Universe::notifyWorldEnd(weak_ptr<World> me)
{
	if(shared_ptr<World> world = me.lock()){
		const int idx = this->worldStack.indexOf(world);
		if(idx < 0){
			log.w(TAG, "oops. notified unknown world.");
			return;
		}
		this->worldStack.erase(idx);

		if(shared_ptr<World> topWorld = this->worldStack.top()){
			// FIXME: 下の画面について、以前よりサイズが変わってるようならreshape
			topWorld->reshape(this->area());
		}
	}else{
		log.w(TAG, "notified world end, but world was already dead.");
	}
}

std::string Universe::resolveWorldFilepath(const std::string& worldname, const std::string& related_filename)
{
	return this->basepath_+util::FileUtil::Sep+worldname+util::FileUtil::Sep+related_filename;
}
std::string Universe::resolveUniverseFilepath(const std::string& related_filename)
{
	return this->basepath_+util::FileUtil::Sep+related_filename;
}

gl::Sprite::Handler Universe::queryImage(const std::string& abs_filename)
{
	return this->canvas_.queryImage(abs_filename);
}

}}
