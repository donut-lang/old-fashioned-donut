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
#include "../util/ClassUtil.h"
#include "../logging/Logger.h"
#include "../geom/Vector.h"

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
	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInScreen){return false;};
	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInScreen){return false;};
	virtual bool onMoveRaw(const float timeMs, const geom::Point& ptInScreen){return false;};
	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInScreen){return false;};
	virtual bool onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity){return false;};
	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance){return false;};
	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio){return false;};
	// 実装用
	//	virtual bool onDownRaw(const float timeMs, const geom::Point& ptInScreen) override;
	//	virtual bool onUpRaw(const float timeMs, const geom::Point& ptInScreen) override;
	//	virtual bool onMoveRaw(const float timeMs, const geom::Point& ptInScreen) override;
	//	virtual bool onSingleTapUp(const float timeMs, const geom::Point& ptInScreen) override;
	//	virtual bool onFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity) override;
	//	virtual bool onScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance) override;
	//	virtual bool onZoom(const float timeMs, const geom::Point& center, const float ratio) override;
};

class GestureSession {
	DISABLE_COPY_AND_ASSIGN(GestureSession);
	DEFINE_MEMBER_REF(private, logging::Logger, log)
public:
	GestureSession(logging::Logger& log, const unsigned int pointerIndex, std::weak_ptr<Layout> targetLayout, const geom::Point& startPoint, const float startTimeMs);
	virtual ~GestureSession();
private:
	//static constexpr float MaxFlingVelocity=4000;
	static constexpr float MinFlingVelocity=5/1000;
private:
	std::weak_ptr<Layout> target_;
	std::deque<std::weak_ptr<Layout> > layoutChain_;
	typedef std::deque<std::weak_ptr<Layout> >::const_iterator LayoutConstIterator;
	typedef std::deque<std::weak_ptr<Layout> >::iterator LayoutIterator;
	const unsigned int pointerIndex_;
	const geom::Point startPoint_;
	const float startTimeMs_;
	geom::Point lastPoint_;
	float lastTimeMs_;
private:
	geom::Distance totalMoved_;
public:
	void onTouchUp(const float timeMs, const geom::Point& pt);
	void onTouchMove(const float timeMs, const geom::Point& pt);
private:
	void invokeDownRaw(const float timeMs, const geom::Point& pt);
	void invokeUpRaw(const float timeMs, const geom::Point& pt);
	void invokeMoveRaw(const float timeMs, const geom::Point& pt);
	void invokeFling(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Velocity& velocity);
	void invokeScroll(const float timeMs, const geom::Point& start, const geom::Point& end, const geom::Distance& distance);
	void invokeZoom(const float timeMs, const geom::Point& center, const float ratio);
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
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
};

}
}

#endif /* INCLUDE_GUARD */
