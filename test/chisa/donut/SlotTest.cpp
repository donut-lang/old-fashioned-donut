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

#include "Helper.h"
#include <math.h>

namespace chisa {
namespace donut {

class DonutSlotTest : public ::testing::Test
{
protected:
	Handler<Donut> donut;
	Handler<Clock> clock;
	Handler<Heap> heap;
public:
	void SetUp(){
		donut = Handler<Donut>(new Donut(log_trace));
		clock = donut->clock();
		heap = donut->heap();
	}
	void TearDown(){
	}
};

TEST_F(DonutSlotTest, NopTest)
{
	Slot slot;
	ASSERT_FALSE(slot.have());
	ASSERT_ANY_THROW(slot.load());
}

TEST_F(DonutSlotTest, RegTest)
{
	Handler<Heap> heap = donut->heap();
	Handler<Object> obj100 = heap->createInt(100);
	Slot slot;
	slot.store(heap, obj100.get());

	ASSERT_TRUE(slot.have());
	ASSERT_NO_THROW(slot.load());
	ASSERT_EQ(obj100->toInt(heap), slot.load()->toInt(heap));

	Handler<Object> obj200 = heap->createInt(200);
	slot.store(heap, obj200.get());

	ASSERT_TRUE(slot.have());
	ASSERT_NO_THROW(slot.load());
	ASSERT_EQ(obj200->toInt(heap), slot.load()->toInt(heap));
}

TEST_F(DonutSlotTest, SeekTest)
{
	Handler<Object> obj100 = heap->createInt(100);
	Slot slot;
	clock->tick();
	slot.store(heap, obj100.get());

	ASSERT_TRUE(slot.have());
	ASSERT_NO_THROW(slot.load());
	ASSERT_EQ(obj100->toInt(heap), slot.load()->toInt(heap));

	clock->tick();
	slot.onSeekNotify(heap);
	Handler<Object> obj200 = heap->createInt(200);
	slot.store(heap, obj200.get());

	ASSERT_TRUE(slot.have());
	ASSERT_NO_THROW(slot.load());
	ASSERT_EQ(obj200->toInt(heap), slot.load()->toInt(heap));

	clock->seek(clock->now()-1);
	slot.onSeekNotify(heap);

	ASSERT_TRUE(slot.have());
	ASSERT_NO_THROW(slot.load());
	ASSERT_EQ(obj100->toInt(heap), slot.load()->toInt(heap));

	clock->seek(clock->firstTime());
	slot.onSeekNotify(heap);
	ASSERT_FALSE(slot.have());
	ASSERT_ANY_THROW(slot.load());
}

TEST_F(DonutSlotTest, SaveTest)
{
	util::XValue v;
	timestamp_t t;
	{
		Slot slot;
		clock->tick();
		const timestamp_t t = clock->now();
		Handler<Object> obj100 = heap->createInt(100);
		slot.store(heap, obj100.get());

		clock->tick();
		slot.onSeekNotify(heap);
		Handler<Object> obj200 = heap->createInt(200);
		slot.store(heap, obj200.get());

		clock->seek(t);
		slot.onSeekNotify(heap);
		v = slot.save();
	}
	{
		Slot slot;
		slot.load(heap, v);
		ASSERT_EQ(100, slot.load()->toInt(heap));
	}


}
}}


