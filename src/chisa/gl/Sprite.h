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

namespace chisa {
namespace gl {
class Canvas;
class RawSprite;

class Sprite {
friend class gl::Handler<Sprite>;
friend class gl::Handler<RawSprite>;
	DISABLE_COPY_AND_ASSIGN(Sprite);
private:
	DEFINE_MEMBER_CONST(public, Canvas*, canvas);
	int refcount_;
public:
	Sprite(Canvas* const canvas);
	virtual ~Sprite();
private:
	void incref();
	void decref();
protected:
	virtual void onFree() = 0;
};

}}

#endif /* SPRITE_H_ */
