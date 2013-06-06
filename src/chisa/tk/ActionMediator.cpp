/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Action.h"
#include "World.h"

namespace chisa {
namespace tk {

static const std::string TAG("GestureMediator");

ActionMediator::ActionMediator(Logger& log, const HandlerW<World> world)
:log_(log)
,world_(world)
,lastSession_(nullptr)
{
	for(size_t i=0;i<MaxTouxhPoint;++i){
		this->session_[i] = nullptr;
	}
}

ActionMediator::~ActionMediator()
{
	if(this->lastSession_){
		delete this->lastSession_;
	}
	for(ActionSession*& session : this->session_) {
		if(session){
			this->log().e(TAG, "Oops. An gesture event was not closed.");
			delete session;
			session = nullptr;
		}
	}
}

void ActionMediator::releaseSession(float const& timeMs, unsigned int const pointerIndex)
{
	ActionSession* const before = this->lastSession_; //can be null
	ActionSession* const after = this->session_[pointerIndex]; //must not null
	this->session_[pointerIndex] = nullptr;
	this->lastSession_ = after;
	if(before){
		before->onFocusLost(timeMs, after);
	}
	after->onFocusGained(timeMs, before);
	if(before){
		delete before;
	}
}

void ActionMediator::onTouchDown(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch down occcured, but there is already a GestureSession for pointerIndex: %d", pointerIndex);
		this->releaseSession(timeMs, pointerIndex);
	}
	if(Handler<World> world = this->world().lock()){
		ActionSession* const session = new ActionSession(this->log(), pointerIndex, world->findElementByPoint(screenPoint), screenPoint, timeMs);
		this->session_[pointerIndex] = session;
	}
}

void ActionMediator::onTouchUp(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(!this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch up occcured, but there is no GestureSession for pointerIndex: %d", pointerIndex);
		return;
	}
	ActionSession* const session = this->session_[pointerIndex];
	this->releaseSession(timeMs, pointerIndex);
	session->onTouchUp(timeMs, screenPoint);
}

void ActionMediator::onTouchMove(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(!this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch move occured, but there is no GestureSession for pointerIndex: %d", pointerIndex);
		return;
	}
	this->session_[pointerIndex]->onTouchMove(timeMs, screenPoint);
}

void ActionMediator::onScroll(float const& timeMs, const float ratio)
{
	if(this->lastSession_){
		this->lastSession_->onScroll(timeMs, ratio);
	}
}

void ActionMediator::onTextInput(float const& timeMs, std::string const& text)
{
	if(this->lastSession_){
		this->lastSession_->onTextInput(timeMs, text);
	}
}
void ActionMediator::onTextEdit(float const& timeMs, std::string const& text, int const start, int const length)
{
	if(this->lastSession_){
		this->lastSession_->onTextEdit(timeMs, text, start, length);
	}
}

void ActionMediator::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	if(this->lastSession_){
		this->lastSession_->onKeyDown(timeMs, isRepeat, sym);
	}
}
void ActionMediator::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	if(this->lastSession_){
		this->lastSession_->onKeyUp(timeMs, sym);
	}
}


}}
