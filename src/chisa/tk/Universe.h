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
#include "Geom.h"
#include "../logging/Logger.h"

namespace chisa {
namespace tk {

/**
 * アプリケーションウィンドウ全体を表すクラス
 */
class Universe {
private:
	logging::Logger& log;
	Stack<shared_ptr<World> > worldStack;
	std::string basepath_;
	Area area_;
	weak_ptr<Universe> self_;
public:
	Universe(logging::Logger& log, const std::string& basepath);
	virtual ~Universe();
public:
	void init(weak_ptr<Universe> _self);
	inline Area area() const{ return area_; };
private:
	inline void area(const Area& newArea) { area_=newArea; };
public:
	void render();
	void idle(const float delta_ms);
	void reshape(const Area& area);
public: //worldからの通知
	void createNewWorld(const string& worldName);
	void notifyWorldEnd(weak_ptr<World> me);
};

}}
#endif /* INCLUDE_GUARD */
