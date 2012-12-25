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

#pragma once
#include <tarte/Handler.h>
#include <tarte/ClassUtil.h>
#include <tarte/Logger.h>
#include "../geom/Vector.h"

namespace chisa {
using namespace tarte;

namespace tk {
class World;
class Element;

class GestureListener {
	DISABLE_COPY_AND_ASSIGN(GestureListener);
public:
	GestureListener() = default;
	virtual ~GestureListener() noexcept = default;
public:
	virtual bool onDownRaw(const float timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onUpRaw(const float timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onMoveRaw(const float timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onSingleTapUp(const float timeMs, geom::Point const& ptInScreen){return false;};
	virtual bool onFling(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity){return false;};
	virtual bool onScroll(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance){return false;};
	virtual bool onZoom(const float timeMs, geom::Point const& center, const float ratio){return false;};
	virtual void onFocusGained(const float timeMs){};
	virtual void onFocusLost(const float timeMs){};
	// 実装用
	//	virtual bool onDownRaw(const float timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onUpRaw(const float timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onMoveRaw(const float timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onSingleTapUp(const float timeMs, geom::Point const& ptInScreen) override;
	//	virtual bool onFling(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity) override;
	//	virtual bool onScroll(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance) override;
	//	virtual bool onZoom(const float timeMs, geom::Point const& center, const float ratio) override;
	// virtual bool onFocusGained(const float timeMs) override;
	// virtual bool onFocusLost(const float timeMs) override;
};

class GestureSession {
	DISABLE_COPY_AND_ASSIGN(GestureSession);
	DEFINE_MEMBER_REF(private, Logger, log)
public:
	GestureSession(Logger& log, const unsigned int pointerIndex, HandlerW<Element> targetElement, geom::Point const& startPoint, const float startTimeMs);
	virtual ~GestureSession();
private:
	//static constexpr float MaxFlingVelocity=4000;
	static constexpr float MinFlingVelocity=500.0f/1000;
private:
	HandlerW<Element> target_;
	std::vector<HandlerW<Element> > elementChain_;
	typedef std::vector<HandlerW<Element> >::iterator ElementIterator;
	const unsigned int pointerIndex_;
	const geom::Point startPoint_;
	const float startTimeMs_;
	geom::Point lastPoint_;
	float lastTimeMs_;
private:
	geom::Distance totalMoved_;
public:
	void onTouchUp(const float timeMs, geom::Point const& pt);
	void onTouchMove(const float timeMs, geom::Point const& pt);
	void onScroll(const float timeMs, float const ratio);
	void onFocusGained(const float timeMs);
	void onFocusLost(const float timeMs);
private:
	void invokeDownRaw(const float timeMs, geom::Point const& pt);
	void invokeUpRaw(const float timeMs, geom::Point const& pt);
	void invokeMoveRaw(const float timeMs, geom::Point const& pt);
	void invokeFling(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Velocity const& velocity);
	void invokeScroll(const float timeMs, geom::Point const& start, geom::Point const& end, geom::Distance const& distance);
	void invokeZoom(const float timeMs, geom::Point const& center, const float ratio);
	void invokeFocusGained(const float timeMs);
	void invokeFocusLost(const float timeMs);
};

class GestureMediator {
	DISABLE_COPY_AND_ASSIGN(GestureMediator);
	DEFINE_MEMBER_REF(private, Logger, log)
	DEFINE_MEMBER_CONST(private, HandlerW<World>, world)
private:
	static constexpr std::size_t MaxTouxhPoint = 5;
	GestureSession* session_[MaxTouxhPoint];
	GestureSession* lastSession_;
private:
	void releaseSession(const float timeMs, unsigned int const pointerIndex);
public:
	GestureMediator(Logger& log, const HandlerW<World> world);
	virtual ~GestureMediator();
public:
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onScroll(const float timeMs, const float ratio);
};

}}
