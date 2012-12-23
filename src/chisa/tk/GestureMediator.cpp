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

#include "Gesture.h"
#include "World.h"

namespace chisa {
namespace tk {

static const std::string TAG("GestureMediator");

GestureMediator::GestureMediator(Logger& log, const HandlerW<World> world)
:log_(log)
,world_(world)
{
	for(size_t i=0;i<MaxTouxhPoint;++i){
		this->session_[i] = nullptr;
	}
}

GestureMediator::~GestureMediator()
{
}

void GestureMediator::onTouchDown(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch down occcured, but there is already a GestureSession for pointerIndex: %d", pointerIndex);
		delete this->session_[pointerIndex];
		this->session_[pointerIndex] = nullptr;
	}
	if(Handler<World> world = this->world().lock()){
		GestureSession* const session = new GestureSession(this->log(), pointerIndex, world->getElementByPoint(screenPoint), screenPoint, timeMs);
		this->session_[pointerIndex] = session;
	}
}

void GestureMediator::onTouchUp(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(!this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch up occcured, but there is no GestureSession for pointerIndex: %d", pointerIndex);
		return;
	}
	GestureSession* const session = this->session_[pointerIndex];
	this->session_[pointerIndex] = nullptr;
	session->onTouchUp(timeMs, screenPoint);
	delete session;
}

void GestureMediator::onTouchMove(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(!this->session_[pointerIndex]){
		this->log().e(TAG, "oops. touch move occured, but there is no GestureSession for pointerIndex: %d", pointerIndex);
		return;
	}
	this->session_[pointerIndex]->onTouchMove(timeMs, screenPoint);
}

}}
