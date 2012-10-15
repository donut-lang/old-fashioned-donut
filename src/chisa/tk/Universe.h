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

#ifndef Chisa_TK_UNIVERSE_H__CPP_
#define Chisa_TK_UNIVERSE_H__CPP_

#include "World.h"
#include "../logging/Logger.h"
#include "../gl/Canvas.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"

namespace chisa {
namespace tk {

/**
 * アプリケーションウィンドウ全体を表すクラス
 */
class Universe {
	DISABLE_COPY_AND_ASSIGN(Universe);
private:
	DEFINE_MEMBER_REF(private, logging::Logger, log);
	Stack<shared_ptr<World> > worldStack;
	std::string basepath_;
	DEFINE_MEMBER(public, private, geom::Area, area);
	weak_ptr<Universe> self_;
	gl::Canvas canvas_;
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const geom::Area& area);
public: //worldからの通知
	void createNewWorld(const string& worldName);
	void notifyWorldEnd(weak_ptr<World> me);
public:
	std::string resolveWorldFilepath(const std::string& worldname, const std::string& related_filename);
	std::string resolveUniverseFilepath(const std::string& related_filename);
	gl::Handler<gl::RawSprite> queryImage(const std::string& abs_filename);
	/******************************************************************************
	 * タッチイベント
	 ******************************************************************************/
public:
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, const geom::Point& screenPoint);
	/******************************************************************************
	 * 生成
	 ******************************************************************************/
private:
	Universe(logging::Logger& log, const std::string& basepath);
	void init(weak_ptr<Universe> _self);
public:
	static shared_ptr<Universe> create(logging::Logger& log, const std::string& basepath)
	{
		shared_ptr<Universe> ptr(new Universe(log, basepath));
		ptr->init(ptr);
		return ptr;
	}
	virtual ~Universe();
};

}}
#endif /* INCLUDE_GUARD */
