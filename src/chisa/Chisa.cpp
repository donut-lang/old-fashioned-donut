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

#include "Chisa.h"

namespace chisa {
const static std::string& TAG("Chisa");

Chisa::Chisa(Logger& log, Handler<PlatformFairy> fairy, Handler<Hexe> hexe)
:log_(log)
,fairy_(fairy)
,hexe_(hexe)
,universe_(tk::Universe::create(this->log(), fairy, hexe))
,isHidden_(false)
{
}

void Chisa::init(std::string const& windowTitle, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits )
{
	fairy_->init(windowTitle, width, height, redbits, greenbits, bluebits, alphabits, depthbits, stencilbits);
	this->reshape(width, height);
	this->universe_->reshape(geom::Area(0,0,width,height));
}

void Chisa::setTitle(std::string const& name)
{
	fairy_->setTitle(name);
}

void Chisa::start(std::string const& initialWorldName)
{
	this->universe_->createNewWorld(initialWorldName);
	this->loop();
}

void Chisa::loop()
{
	bool running = true;
	float last = fairy_->getTimeMs();
	float delta = 0.0f;
	float nextFrame = last+(1000.0f/60);
	this->fairy_->quartet()->start();

	while(running){
		this->universe_->idle(delta);
		float now = fairy_->getTimeMs();
		delta = now-last;
		last = now;
		if(now < nextFrame){
			if( likely(!isHidden_) ){
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				this->universe_->render();
				this->fairy_->swapBuffer();
				now = fairy_->getTimeMs();
				if(now < nextFrame){
					fairy_->sleepMs(nextFrame-now);
				}
			}else{
				fairy_->sleepMs(nextFrame-now);
			}
		}
		nextFrame+=(1000.0/60);
		running &= this->fairy_->pollEvent(*this);
	}
	if(this->log().i()){
		this->log().i(TAG, "Shutting down...");
	}
}

void Chisa::onShown()
{
	if(this->log().d()){
		this->log().d(TAG, "onShown");
	}
	this->isHidden_ = false;
}
void Chisa::onHidden()
{
	if(this->log().d()){
		this->log().d(TAG, "onHidden");
	}
	this->isHidden_ = true;
}
void Chisa::reshape(float const& width, float const& height)
{
	if(!isHidden_){
		this->universe_->reshape(geom::Area(0,0,width,height));
	}
}
void Chisa::pointerDown(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint)
{
	this->universe_->onTouchDown(timeMs, pointerIndex, screenPoint);
}
void Chisa::pointerUp(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint)
{
	this->universe_->onTouchUp(timeMs, pointerIndex, screenPoint);
}
void Chisa::pointerMove(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint)
{
	this->universe_->onTouchMove(timeMs, pointerIndex, screenPoint);
}
void Chisa::pointerScroll(float const& timeMs, float const& ratio)
{
	this->universe_->onScroll(timeMs, ratio);
}

void Chisa::textInput(float const& timeMs, std::string const& text)
{
	this->universe_->onTextInput(timeMs, text);
}
void Chisa::textEdit(float const& timeMs, std::string const& text, int const& start, int const& length)
{
	this->universe_->onTextEdit(timeMs, text, start, length);
}
void Chisa::keyDown(float const& timeMs, const bool isRepeat, SDL_Keysym const& sym)
{
	this->universe_->onKeyDown(timeMs, isRepeat, sym);
}
void Chisa::keyUp(float const& timeMs, SDL_Keysym const& sym)
{
	this->universe_->onKeyUp(timeMs, sym);
}


}
