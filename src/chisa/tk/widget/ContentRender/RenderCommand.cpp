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

namespace chisa {
namespace tk {
namespace widget {

Handler<gl::Sprite> SpriteRenderCommand::realize(gl::Canvas& cv) {
	if(!this->sprite()){
		this->sprite(this->realizeImpl(cv));
	}
	return this->sprite();
}
void SpriteRenderCommand::onHidden() noexcept{
	this->sprite().reset();
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
