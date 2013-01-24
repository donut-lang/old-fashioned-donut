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
#include "TwinAngel.h"
#include "../donut/Patron.h"
#include "../../gl/DecoUtil.h"

namespace chisa {
namespace tk {

TwinAngel::TwinAngel(Handler<Heaven> const& heaven)
:Angel(heaven)
{
}

void TwinAngel::renderImpl(gl::Canvas& canvas)
{
	if(this->targets().size() != 2) {
		return;
	}
	Handler<AngelTarget> const& t1 = this->targets()[0];
	Handler<AngelTarget> const& t2 = this->targets()[1];
	geom::Area const a1 = t1->findScreenArea();
	geom::Area const a2 = t2->findScreenArea();

	if(a1.empty() || a2.empty()) {
		return;
	}

	if( (a1.right() + a1.left() / 2) < (a2.right() + a2.left() / 2) ) {
		gl::renderCubicLine(canvas, 3, gl::Color(1.0f,.0f,.0f,.3f), geom::Point(a1.right(), a1.top()+a1.height()/2), geom::Point(a2.left(), a2.top()+a2.height()/2));
	}else{
		gl::renderCubicLine(canvas, 3, gl::Color(1.0f,.0f,.0f,.3f), geom::Point(a1.left(), a1.top()+a1.height()/2), geom::Point(a2.right(), a2.top()+a2.height()/2));
	}
}

void TwinAngel::idleImpl(const float delta_ms)
{
}

void TwinAngel::reshapeImpl(const geom::Area& area)
{
}

Handler<TwinAngel> TwinAngel::self() noexcept
{
	return Handler<TwinAngel>::__internal__fromRawPointerWithoutCheck(this);
}

Handler< ::donut::Object> TwinAngel::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return Handler< ::donut::Object>( world()->patron()->twinAngelProvider()->newInstance(heap, self()) );
}

Handler<AngelTarget> TwinAngel::attatchTarget(const Handler<AngelTarget>& target)
{
	if(numTargets() >= 2) {
		TARTE_EXCEPTION(Exception, "[BUG] Oops. Twin angel can hold only two targets.")
	}
	return this->Angel::attatchTarget(target);
}

Handler<AngelTarget> TwinAngel::detatchTarget(const Handler<AngelTarget>& target)
{
	return this->Angel::detatchTarget(target);
}


}}
