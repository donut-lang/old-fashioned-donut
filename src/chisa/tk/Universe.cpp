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

#include <tarte/FileSystem.h>

#include "Universe.h"
#include "World.h"
#include "../input/JoystickManager.h"
#include "../gl/DrawableManager.h"

static const std::string TAG("Universe");

namespace chisa {
namespace tk {

Universe::Universe(Logger& log, Handler<PlatformFairy> fairy, Handler<Hexe> hexe)
:log_(log)
,platformFairy_(fairy)
,hexe_(hexe)
,canvas_(log)
,drawableManager_( new gl::DrawableManager(log, gl::DrawableSetting(this->resolveUniverseFilepath("__font__"))) )
{
}
Universe::~Universe() noexcept{
}

void Universe::init()
{
}

void Universe::render()
{
	//TODO: 下のスタックについて、オフスクリーンレンダリングしたほうがいい？？
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->render(this->canvas_);
	}
}
void Universe::idle(const float delta_ms)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->idle(delta_ms);
	}
}
void Universe::reshape(geom::Area const& area)
{
	if(log().t()){
		log().t(TAG, "reshaped: %s", area.toString().c_str());
	}
	this->canvas_.resize2d(area.box());
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->reshape(geom::Area(0,0, area.width(), area.height()));
	}
	this->area(area);
}

void Universe::createNewWorld(std::string const& worldName)
{
	Handler<World> newWorld(World::create(log(),this->self(), worldName));
	newWorld->reshape(this->area());
	this->worldStack.push(newWorld);
}

void Universe::notifyWorldEnd(HandlerW<World> me)
{
	if(Handler<World> world = me.lock()){
		const int idx = this->worldStack.indexOf(world);
		if(idx < 0){
			log().w(TAG, "oops. notified unknown world.");
			return;
		}
		this->worldStack.erase(idx);

		if(Handler<World> topWorld = this->worldStack.top()){
			// FIXME: 下の画面について、以前よりサイズが変わってるようならreshape
			topWorld->reshape(this->area());
		}
	}else{
		log().w(TAG, "notified world end, but world was already dead.");
	}
}
void Universe::onTouchDown(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onTouchDown(timeMs, pointerIndex, screenPoint);
	}
}

void Universe::onTouchUp(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onTouchUp(timeMs, pointerIndex, screenPoint);
	}
}

void Universe::onTouchMove(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onTouchMove(timeMs, pointerIndex, screenPoint);
	}
}

void Universe::onScroll(float const& timeMs, const float delta)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onScroll(timeMs, delta);
	}
}

void Universe::onTextInput(float const& timeMs, std::string const& text)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onTextInput(timeMs, text);
	}
}
void Universe::onTextEdit(float const& timeMs, std::string const& text, int start, int length)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onTextEdit(timeMs, text, start, length);
	}
}

void Universe::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onKeyDown(timeMs, isRepeat, sym);
	}
}
void Universe::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	if(Handler<World> topWorld = this->worldStack.top()){
		topWorld->onKeyUp(timeMs, sym);
	}
}

void Universe::startIME(geom::Area const& area)
{
	this->platformFairy()->startIME(area);
}
void Universe::stopIME()
{
	this->platformFairy()->stopIME();
}

/******************************************************************************
 * サウンド
 ******************************************************************************/
Handler<Quartet> Universe::quartet() const
{
	return this->platformFairy()->quartet();
}

Handler<JoystickManager> Universe::joystickManager() const
{
	return this->platformFairy_->joystickManager();
}
Handler<chisa::WorldGeist> Universe::invokeWorldGeist(HandlerW<World> world, std::string const& nameOfGeist)
{
	if(log().t()){
		log().t(TAG, "Invoking: %s",nameOfGeist.c_str());
	}
	return this->hexe()->invokeWorldGeist(world, nameOfGeist);
}

Handler<gl::DrawableManager> Universe::drawableManager() const
{
	return this->drawableManager_;
}


}}
