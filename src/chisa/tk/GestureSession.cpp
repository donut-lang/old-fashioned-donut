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
#include "Element.h"
#include <algorithm>

namespace chisa {
namespace tk {

static const std::string TAG("GestureSession");

GestureSession::GestureSession(logging::Logger& log, const unsigned int pointerIndex, std::weak_ptr<Element> targetElement, const geom::Point& startPoint, const float startTimeMs)
:log_(log)
,target_(targetElement)
,pointerIndex_(pointerIndex)
,startPoint_(startPoint)
,startTimeMs_(startTimeMs)
,lastPoint_(startPoint)
,lastTimeMs_(startTimeMs)
,totalMoved_(0,0)
{
	const std::shared_ptr<Element> orig_target = targetElement.lock();
	if(!orig_target){
		log.e(TAG, "[Touch Session %d] oops. Target Element was already deleted.", this->pointerIndex_);
		return;
	}
	//このセッションに関わるレイアウトを列挙
	std::shared_ptr<Element> _it = orig_target;
	while( _it ) {
		this->elementChain_.push_front( _it );
		_it = _it->parent().lock();
	}
	for(ElementIterator it = this->elementChain_.begin(); it != this->elementChain_.end(); ++it){
		std::shared_ptr<Element> target = it->lock();
		if(log.t()){
			log.t(TAG, "Touch Session creating: %s at %f index: %d element: %s", startPoint.toString().c_str(), startTimeMs, pointerIndex, target->toString().c_str());
		}
		if(target->onDownRaw(this->startTimeMs_, this->startPoint_)){
			//onDownイベントをconsumeした先には一切イベントを分け与えない。
			if(it+1 != this->elementChain_.end()){
				this->elementChain_.erase(it+1, this->elementChain_.end());
				std::weak_ptr<Element>().swap(this->target_);
			}
			break;
		}
	}
}

GestureSession::~GestureSession()
{
}

void GestureSession::onTouchUp(const float timeMs, const geom::Point& pt)
{
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
	this->invokeUpRaw(timeMs, pt);

	this->totalMoved_ += (pt-this->lastPoint_);

	const float timeDiff = this->lastTimeMs_-this->startTimeMs_;
	const geom::Velocity vel(this->totalMoved_, timeDiff);
	if(fabs(vel.x()) > MinFlingVelocity || fabs(vel.y()) > MinFlingVelocity ){
		this->invokeFling(timeMs, this->startPoint_, pt, vel);
		return;
	}
	if( std::shared_ptr<Element> target = this->target_.lock() ){
		if(target->screenArea().contain(pt)){
			target->onSingleTapUp(timeMs, pt);
			return;
		}
	}
}

void GestureSession::onTouchMove(const float timeMs, const geom::Point& pt)
{
	this->invokeMoveRaw(timeMs, pt);
	this->invokeScroll(timeMs, this->lastPoint_, pt, pt-this->lastPoint_);
	this->totalMoved_ += (pt-this->lastPoint_);
	this->lastTimeMs_ = timeMs;
	this->lastPoint_ = pt;
}

void GestureSession::invokeDownRaw(const float timeMs, const geom::Point& pt)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session creating: %s at %f index: %d element: %s", pt.toString().c_str(), this->startTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onDownRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeUpRaw(const float timeMs, const geom::Point& pt)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(this->log().t()){
				this->log().t(TAG, "Touch Session ending: %s at %f index: %d element: %s", pt.toString().c_str(), this->lastTimeMs_, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onUpRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeMoveRaw(const float timeMs, const geom::Point& pt)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(target->onMoveRaw(timeMs, pt)){
				break;
			}
		}
	}
}

void GestureSession::invokeFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(target->onFling(timeMs, start, end, velocity)){
				break;
			}
		}
	}
}

void GestureSession::invokeScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(target->onScroll(timeMs, start, end, distance)){
				break;
			}
		}
	}
}

void GestureSession::invokeZoom(const float timeMs, const geom::Point& center, const float ratio)
{
	for(std::weak_ptr<Element> const& it : this->elementChain_) {
		if(std::shared_ptr<Element> target = it.lock()){
			if(target->onZoom(timeMs, center, ratio)){
				break;
			}
		}
	}
}

}}
