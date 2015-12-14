/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Chisa.hpp"

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

void Chisa::init(std::string const& windowTitle, int width, int height, bool isFullScreen, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits )
{
	fairy_->init(windowTitle, width, height, isFullScreen, redbits, greenbits, bluebits, alphabits, depthbits, stencilbits);
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

void Chisa::onMouseBack(float const& timeMs)
{
	this->universe_->onMouseBack(timeMs);
}

void Chisa::onMouseForward(float const& timeMs)
{
	this->universe_->onMouseForward(timeMs);
}

void Chisa::keyUp(float const& timeMs, SDL_Keysym const& sym)
{
	this->universe_->onKeyUp(timeMs, sym);
}


}
