/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/object/Heap.h>
#include <donut/object/HomuraObject.h>
#include <donut/provider/HomuraProvider.h>
#include <donut/Exception.h>

namespace donut {

static const std::string TAG("HomuraProvider");

HomuraProvider::HomuraProvider(Handler<Heap> const& heap)
:Super(heap, "Homura")
{
	this->registerPureNativeClosure("tick", [&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		timestamp_t const t = clk->now();
		clk->tickFromMachine();
		return t;
	});
	this->registerPureNativeClosure("now", [&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		return heap->clock()->now();
	});
	this->registerPureNativeClosure("seek", [&](HomuraObject* hom, timestamp_t t) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		clk->seekFromMachine(t);
		return t;
	});
	this->registerPureNativeClosure("discardHistory", [&](HomuraObject* hom) {
		Handler<Heap> heap = this->heap().lock();
		if(!heap){
			DONUT_EXCEPTION(Exception, "[BUG] Heap is already dead.");
		}
		Handler<Clock> clk(heap->clock());
		clk->discardHistoryFromMachine();
		return clk->now();
	});
}

}
