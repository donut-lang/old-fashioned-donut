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
#include <tarte/Logger.h>
#include "../gl/Canvas.h"
#include "../gl/Font.h"
#include "../geom/Area.h"
#include "../geom/Vector.h"
#include "../Hexe.h"
#include "Stack.h"

namespace chisa {
using namespace tarte;

namespace gl {
class DrawableManager;
}

namespace tk {

class World;

/**
 * アプリケーションウィンドウ全体を表すクラス
 */
class Universe : public HandlerBody<Universe> {
	DISABLE_COPY_AND_ASSIGN(Universe);
private:
	DEFINE_MEMBER_REF(private, Logger, log);
	DEFINE_MEMBER_CONST(public, Hexe*, hexe);
	Stack<Handler<World> > worldStack;
	DEFINE_MEMBER(public, private, geom::Area, area);
	gl::Canvas canvas_;
	Handler<gl::DrawableManager> drawableManager_;
public:
	void render();
	void idle(const float delta_ms);
	void reshape(geom::Area const& area);
public: //worldからの通知
	void createNewWorld(std::string const& worldName);
	void notifyWorldEnd(HandlerW<World> me);
	/******************************************************************************
	 * Hexeへ移譲
	 ******************************************************************************/
public:
	template <typename... Args>
	constexpr std::string resolveWorldFilepath(std::string const& worldname, Args const&... related_filename) const noexcept
	{
		return this->hexe()->resolveFilepath(worldname, related_filename...);
	}
	template <typename... Args>
	constexpr std::string resolveUniverseFilepath(Args const&... related_filename) const
	{
		return this->hexe()->resolveFilepath(related_filename...);
	}
	Handler<chisa::WorldGeist> invokeWorldGeist(HandlerW<World> world, std::string const& nameOfGeist);
	/******************************************************************************
	 * 画像
	 ******************************************************************************/
public:
	Handler<gl::DrawableManager> drawableManager() const;
	/******************************************************************************
	 * タッチイベント
	 ******************************************************************************/
public:
	void onTouchDown(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchUp(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	void onTouchMove(const float timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint);
	/******************************************************************************
	 * 生成
	 ******************************************************************************/
private:
	Universe(Logger& log, Hexe* hexe);
	void init();
public:
	static Handler<Universe> create(Logger& log, Hexe* hexe)
	{
		Handler<Universe> ptr(new Universe(log, hexe));
		ptr->init();
		return ptr;
	}
	virtual ~Universe() noexcept;
	inline bool onFree() noexcept { return false; };
};

}}
