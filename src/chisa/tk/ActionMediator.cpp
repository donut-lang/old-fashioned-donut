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
	if(this->lastSession_){
		this->lastSession_->onFocusLost(timeMs);
		delete this->lastSession_;
	}
	this->lastSession_ = this->session_[pointerIndex];
	this->session_[pointerIndex] = nullptr;
	this->lastSession_->onFocusGained(timeMs);
}

void ActionMediator::onTouchDown(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch down occcured, but there is already a GestureSession for pointerIndex: %d", pointerIndex);
		this->releaseSession(timeMs, pointerIndex);
	}
	if(Handler<World> world = this->world().lock()){
		ActionSession* const session = new ActionSession(this->log(), pointerIndex, world->getElementByPoint(screenPoint), screenPoint, timeMs);
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


}}