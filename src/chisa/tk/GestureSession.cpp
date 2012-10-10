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

namespace chisa {
namespace tk {

static const std::string TAG("GestureSession");

GestureSession::GestureSession(logging::Logger& log, const unsigned int pointerIndex, std::weak_ptr<Layout> targetLayout, const Point& startPoint, const float startTimeMs)
:log_(log)
,pointerIndex_(pointerIndex)
,targetLayout_(targetLayout)
,startPoint_(startPoint)
,startTimeMs_(startTimeMs)
,gotOutOfRegion(false)
,lastPoint_(startPoint)
,lastTimeMs_(startTimeMs)
{
	if(log.t()){
		log.t(TAG, "Touch Session created: %s at %f index: %d layout: %s", startPoint.toString().c_str(), startTimeMs, pointerIndex, targetLayout.lock()->toString().c_str());
	}
}

GestureSession::~GestureSession()
{
}

void GestureSession::onTouchUp(const float timeMs, const Point& pt)
{
	if(log().t()){
		log().t(TAG, "Touch Session end: %s at %f index: %d layout: %s", pt.toString().c_str(), timeMs, this->pointerIndex_, this->targetLayout_.lock()->toString().c_str());
	}
}

void GestureSession::onTouchMove(const float timeMs, const Point& pt)
{
}



}}
