/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "../Heaven.h"
#include "LoneAngel.h"
#include "../donut/Patron.h"

namespace chisa {
namespace tk {

LoneAngel::LoneAngel(Handler<Heaven> const& heaven)
:Angel(heaven)
{
}

void LoneAngel::renderImpl(gl::Canvas& canvas)
{
}

void LoneAngel::idleImpl(const float delta_ms)
{
}

void LoneAngel::reshapeImpl(const geom::Area& area)
{
}

Handler<LoneAngel> LoneAngel::self() noexcept
{
	return Handler<LoneAngel>::__internal__fromRawPointerWithoutCheck(this);
}


Handler< ::donut::Object> LoneAngel::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->loneAngelProvider()->newInstance(heap, self()) );
}

Handler<AngelTarget> LoneAngel::attatchTarget(const Handler<AngelTarget>& target)
{
	if(numTargets() > 0) {
		CINAMO_EXCEPTION(Exception, "[BUG] Oops. Lone angel can hold only one target.")
	}
	return this->Angel::attatchTarget(target);
}

Handler<AngelTarget> LoneAngel::detatchTarget(const Handler<AngelTarget>& target)
{
	return this->Angel::detatchTarget(target);
}

}}
