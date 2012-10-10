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
#include "Layout.h"
#include <algorithm>

namespace chisa {
namespace tk {

static const std::string TAG("GestureSession");

GestureSession::GestureSession(logging::Logger& log, const unsigned int pointerIndex, std::weak_ptr<Layout> targetLayout, const Point& startPoint, const float startTimeMs)
:log_(log)
,pointerIndex_(pointerIndex)
,startPoint_(startPoint)
,startTimeMs_(startTimeMs)
,gotOutOfRegion_(false)
,lastPoint_(startPoint)
,lastTimeMs_(startTimeMs)
{
	const shared_ptr<Layout> target = targetLayout.lock();
	if(!target){
		log.e(TAG, "[Touch Session %d] oops. Target Layout was already deleted.", this->pointerIndex_);
		return;
	}
	//このセッションに関わるレイアウトを列挙
	shared_ptr<Layout> _it = target;
	while(_it) {
		this->layoutChain_.push_front(_it);
		_it = _it->parent().lock();
	}
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		shared_ptr<Layout> target = it->lock();
		if(log.t()){
			log.t(TAG, "Touch Session creating: %s at %f index: %d layout: %s", startPoint.toString().c_str(), startTimeMs, pointerIndex, target->toString().c_str());
		}
		if(target->onDownRaw(this->startTimeMs_, this->startPoint_)){
			//onDownイベントをconsumeした先には一切イベントを分け与えない。
			this->layoutChain_.erase(it+1, this->layoutChain_.end());
			break;
		}
	}
}

GestureSession::~GestureSession()
{
}

void GestureSession::onTouchUp(const float timeMs, const Point& pt)
{
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
	this->invokeUpRaw(timeMs, pt);
}

void GestureSession::onTouchMove(const float timeMs, const Point& pt)
{
	this->invokeScroll(timeMs, this->lastPoint_, pt, pt-this->lastPoint_);
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
}

void GestureSession::invokeDownRaw(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session creating: %s at %f index: %d layout: %s", pt.toString().c_str(), this->startTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onDownRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeUpRaw(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session ending: %s at %f index: %d layout: %s", pt.toString().c_str(), this->lastTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onUpRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeMoveRaw(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onMoveRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeSingleTapUp(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onSingleTapUp(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeShowPress(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onShowPress(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeFling(const float timeMs, const Point& start, const Point& end, const Velocity& velocity)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onFling(timeMs, start, end, velocity)){
				break;
			}
		}
	}
}

void GestureSession::invokeScroll(const float timeMs, const Point& start, const Point& end, const Distance& distance)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onScroll(timeMs, start, end, distance)){
				break;
			}
		}
	}
}

void GestureSession::invokeZoom(const float timeMs, const Point& center, const float ratio)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onZoom(timeMs, center, ratio)){
				break;
			}
		}
	}
}

}}
