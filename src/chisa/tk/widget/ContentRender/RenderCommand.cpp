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

#include "../../../gl/Canvas.h"
#include "RenderCommand.h"
#include "RenderCache.h"

namespace chisa {
namespace tk {
namespace widget {

Handler<gl::Sprite> SpriteRenderCommand::realize(gl::Canvas& cv) {
	if(!this->sprite()){
		if(this->spritew().expired()){ //キャッシュからも落ちてしまったので、再度構成しなければならない
			this->sprite(this->realizeImpl(cv));
			this->spritew().reset();
		}else{ //まだキャッシュに載ってた
			this->sprite(this->spritew().lock());
			this->cache().unregisterSprite(this->sprite());
			this->spritew().reset();
		}
	}
	return this->sprite();
}
void SpriteRenderCommand::onHidden() noexcept{
	this->cache().registerSprite(this->sprite());
	this->spritew(this->sprite());
	this->sprite().reset();
}

void SpriteRenderCommand::invalidate() noexcept
{
	this->sprite().reset();
	this->spritew().reset();
}

void SpriteRenderCommand::execute(gl::Canvas& canvas, const geom::Point& offset)
{
	canvas.drawSprite(this->realize(canvas), this->area().point() - offset);
}

//-----------------------------------------------------------------------------
Handler<gl::Sprite> TextRenderCommand::realizeImpl(gl::Canvas& cv)
{
	return this->cmd_.renderString(cv);
}

//-----------------------------------------------------------------------------

//	const geom::Point pt(this->area().point() - offset +geom::Vector(0, area().height()));
//	canvas.drawLine(this->width_, this->color_, pt, pt+geom::Vector(area().width(), 0) );

}}}
