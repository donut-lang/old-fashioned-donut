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
#include "../logging/Logger.h"
#include "../gl/Canvas.h"
#include "../gl/Font.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "../Hexe.h"
#include "Stack.h"

namespace chisa {
namespace gl {
class DrawableManager;
}

namespace tk {

class World;

/**
 * アプリケーションウィンドウ全体を表すクラス
 */
class Universe {
	DISABLE_COPY_AND_ASSIGN(Universe);
private:
	DEFINE_MEMBER_REF(private, logging::Logger, log);
	DEFINE_MEMBER_CONST(public, Hexe*, hexe);
	Stack<std::shared_ptr<World> > worldStack;
	DEFINE_MEMBER(public, private, geom::Area, area);
	std::weak_ptr<Universe> self_;
	gl::Canvas canvas_;
	Handler<gl::DrawableManager> drawableManager_;
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const geom::Area& area);
public: //worldからの通知
	void createNewWorld(const std::string& worldName);
	void notifyWorldEnd(std::weak_ptr<World> me);
	/******************************************************************************
	 * Hexeへ移譲
	 ******************************************************************************/
public:
	template <typename... Args>
	constexpr std::string resolveWorldFilepath(const std::string& worldname, const Args&... related_filename) const noexcept
	{
		return this->hexe()->resolveFilepath(worldname, related_filename...);
	}
	template <typename... Args>
	constexpr std::string resolveUniverseFilepath(const Args&... related_filename) const
	{
		return this->hexe()->resolveFilepath(related_filename...);
	}
	std::shared_ptr<chisa::WorldGeist> invokeWorldGeist(std::weak_ptr<tk::World> world, const std::string& nameOfGeist);
	/******************************************************************************
	 * 画像
	 ******************************************************************************/
public:
	Handler<gl::DrawableManager> drawableManager() const;
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
	Universe(logging::Logger& log, Hexe* hexe);
	void init(std::weak_ptr<Universe> _self);
public:
	static std::shared_ptr<Universe> create(logging::Logger& log, Hexe* hexe)
	{
		std::shared_ptr<Universe> ptr(new Universe(log, hexe));
		ptr->init(ptr);
		return ptr;
	}
	virtual ~Universe();
};

}}
