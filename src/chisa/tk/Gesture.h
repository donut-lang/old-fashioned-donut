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

#ifndef _CXX_Chisa_GESTUREMEDIATOR_H_
#define _CXX_Chisa_GESTUREMEDIATOR_H_

#include <memory>
#include <deque>
#include "Geom.h"
#include "../util/class_utils.h"
#include "../logging/Logger.h"

namespace chisa {
namespace tk {
class World;
class Layout;

class GestureListener {
	DISABLE_COPY_AND_ASSIGN(GestureListener);
public:
	GestureListener(){};
	virtual ~GestureListener(){};
public:
	virtual bool onDownRaw(const Point& pt){return false;};
	virtual bool onUpRaw(const Point& pt){return false;};
	virtual bool onMoveRaw(const Point& pt){return false;};
	virtual bool onSingleTapUp(const Point& pt){return false;};
	virtual bool onShowPress(const Point& pt){return false;};
	virtual bool onFling(const Point& start, const Point& end, const Velocity& velocity){return false;};
	virtual bool onScroll(const Point& start, const Point& end, const Distance& distance){return false;};
	virtual bool onZoom(const Point& center, const float ratio){return false;};
};

class GestureSession {
	DISABLE_COPY_AND_ASSIGN(GestureSession);
	DEFINE_MEMBER_REF(private, logging::Logger, log)
public:
	GestureSession(logging::Logger& log, const unsigned int pointerIndex, std::weak_ptr<Layout> targetLayout, const Point& startPoint, const float startTimeMs);
	virtual ~GestureSession();
private:
	std::deque<std::weak_ptr<Layout> > layoutChain_;
	typedef std::deque<std::weak_ptr<Layout> >::const_iterator LayoutConstIterator;
	typedef std::deque<std::weak_ptr<Layout> >::iterator LayoutIterator;
	bool handled_;
	const unsigned int pointerIndex_;
	const Point startPoint_;
	const float startTimeMs_;
	bool gotOutOfRegion_;
	Point lastPoint_;
	float lastTimeMs_;
public:
	void onTouchUp(const float timeMs, const Point& pt);
	void onTouchMove(const float timeMs, const Point& pt);
};

class GestureMediator {
	DISABLE_COPY_AND_ASSIGN(GestureMediator);
	DEFINE_MEMBER_REF(private, logging::Logger, log)
	DEFINE_MEMBER_CONST(private, std::weak_ptr<World>, world)
private:
	static constexpr std::size_t MaxTouxhPoint = 5;
	GestureSession* session_[MaxTouxhPoint];
public:
	GestureMediator(logging::Logger& log, const std::weak_ptr<World> world);
	virtual ~GestureMediator();
public:
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, const Point& screenPoint);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, const Point& screenPoint);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, const Point& screenPoint);
};

}
}

#endif /* INCLUDE_GUARD */
