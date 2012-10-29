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
#include "../util/ClassUtil.h"
#include "../Handler.h"
#include "../geom/Decl.h"
#include "../util/Thread.h"

namespace chisa {
namespace gl {
class Canvas;
class RawSprite;

class Sprite {
	DISABLE_COPY_AND_ASSIGN(Sprite);
	template <typename T> friend class chisa::Handler;
	friend class Canvas;
private:
	int refcount_;
	std::mutex ref_mutex_;
	DEFINE_MEMBER_CONST(public, Canvas*, canvas);
public:
	Sprite(Canvas* const canvas);
	virtual ~Sprite();
private: /* from Handler */
	void incref();
	void decref();
	virtual void onFree() = 0;
private: /* from Canvas */
	virtual void drawImpl(const geom::Point& pt, const float depth=0.0f) = 0;
};

}}
