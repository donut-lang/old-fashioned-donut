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
,handled_(false)
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
		if(target->onDownRaw(startPoint)){
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
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(log().t()){
				log().t(TAG, "Touch Session ending: %s at %f index: %d layout: %s", pt.toString().c_str(), timeMs, this->pointerIndex_, target->toString().c_str());
			}
			if(target->onUpRaw(pt)){
				break;
			}
		}
	}
}

void GestureSession::onTouchMove(const float timeMs, const Point& pt)
{
	for(LayoutIterator it = this->layoutChain_.begin(); it != this->layoutChain_.end(); ++it){
		if(shared_ptr<Layout> target = it->lock()){
			if(target->onMoveRaw(pt)){
				break;
			}
		}
	}
}



}}
