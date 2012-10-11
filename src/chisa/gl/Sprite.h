/*
 * Sprite.h
 *
 *  Created on: Oct 9, 2012
 *      Author: psi
 */

#ifndef _CHISA_GL_SPRITE_H_
#define _CHISA_GL_SPRITE_H_

#include "../util/class_utils.h"
#include "Handler.h"
#include "../geom/Decl.h"

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

#endif /* SPRITE_H_ */
