/*
 * Sprite.h
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#pragma once
#include "../util/ClassUtil.h"
#include "Handler.h"
#include "../geom/Decl.h"
#include "../util/Thread.h"

namespace chisa {
namespace gl {
class Canvas;
class RawSprite;

class Sprite {
	DISABLE_COPY_AND_ASSIGN(Sprite);
	template <typename T> friend class gl::Handler;
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
