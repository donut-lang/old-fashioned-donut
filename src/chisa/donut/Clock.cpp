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

#include "Clock.h"
#include "Donut.h"

namespace chisa {
namespace donut {

static const std::string TAG("Clock");

Clock::Clock( Donut* const donut)
:log_(donut->log())
,now_(1)
,donut_(donut)
{
}

void Clock::discardFuture()
{
	Handler<Donut> donut = this->donut_.lock();
	if(!donut){
		log().e(TAG, "Tried to discard future, but donut was already dead.");
		return;
	}
	donut->discardFuture();
}
void Clock::discardHistory()
{
	Handler<Donut> donut = this->donut_.lock();
	if(!donut){
		log().e(TAG, "Tried to discard history, but donut was already dead.");
		return;
	}
	donut->discardHistory();
}


}}
