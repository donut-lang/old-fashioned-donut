/**
 * Donut
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

#include <donut/object/Heap.h>
#include <donut/object/HomuraObject.h>
#include <donut/provider/HomuraProvider.h>

namespace donut {

static const std::string TAG("HomuraProvider");

HomuraProvider::HomuraProvider(Handler<Heap> const& heap)
:Super(heap, "Homura")
{
	this->registerPureNativeClosure("tick", std::function<timestamp_t(HomuraObject*)>([&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		timestamp_t const t = clk->now();
		clk->tickFromMachine();
		return t;
	}));
	this->registerPureNativeClosure("now", std::function<timestamp_t(HomuraObject*)>([&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		return heap->clock()->now();
	}));
	this->registerPureNativeClosure("seek", std::function<timestamp_t(HomuraObject*, timestamp_t t)>([&](HomuraObject* hom, timestamp_t t) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		clk->seekFromMachine(t);
		return t;
	}));
	this->registerPureNativeClosure("discardHistory", std::function<timestamp_t(HomuraObject*)>([&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		clk->discardHistoryFromMachine();
		return clk->now();
	}));
}

}
