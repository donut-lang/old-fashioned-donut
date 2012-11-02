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

#include "RenderContext.h"
#include "RenderCommand.h"

namespace chisa {
namespace tk {
namespace widget {

RenderContext::RenderContext(logging::Logger& log)
:log_(log)
,cache_(new RenderCache(log))
{
	// TODO Auto-generated constructor stub

}

Handler<RenderCommand> RenderContext::createText(const geom::Area& area, const gl::StringRenderer::Command& cmd)
{
	return Handler<RenderCommand>(new TextRenderCommand(this->cache_, area, cmd));
}
Handler<RenderCommand> RenderContext::createDrawable(const geom::Area& area, const std::string& drawableRepl)
{
	return Handler<RenderCommand>(new DrawableRenderCommand(this->cache_, area, drawableRepl));
}


}}}
