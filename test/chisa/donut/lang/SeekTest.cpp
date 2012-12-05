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

#include "DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {

TEST(SeekTest, TimeShouldAdvanceTest)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_FALSE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	ASSERT_TRUE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );
	unsigned int const time2 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time3 = donut->nowTime();

	ASSERT_LT(time1, time2);
	ASSERT_LT(time2, time3);

	ASSERT_TRUE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );
}

TEST(SeekTest, SeekBefore)
{
	INIT_DONUT
	Handler<Object> result;
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time2 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time3 = donut->nowTime();

	//ここまでの状態を確認
	ASSERT_TRUE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );

	{
		donut->seek( time1 );
		ASSERT_FALSE( donut->canBack() );
		ASSERT_TRUE( donut->canAdvance() );

		ASSERT_FALSE( heap->hasGlobalObject("test") );
	}

	{
		donut->seek( time2 );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_TRUE( donut->canAdvance() );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_TRUE( donut->canAdvance() );
	}

	{
		donut->seek( time3 );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_FALSE( donut->canAdvance() );

		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap) );
	}
}

TEST(SeekTest, SeekAndExecute)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_FALSE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	ASSERT_TRUE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );
	unsigned int const time2 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time3 = donut->nowTime();

	ASSERT_LT(time1, time2);
	ASSERT_LT(time2, time3);

	ASSERT_TRUE( donut->canBack() );
	ASSERT_FALSE( donut->canAdvance() );

	{
		donut->seek( time2 );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_TRUE( donut->canAdvance() );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_TRUE( donut->canAdvance() );

		result = machine->start( donut->parse("Global.test;", "<MEM>", 0) );
		ASSERT_TRUE( donut->canBack() );
		ASSERT_FALSE( donut->canAdvance() );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
}

TEST(SeekTest, SeekWithMultMachineTest)
{
	INIT_DONUT;
	Handler<Machine> emachine = donut->queryMachine("another");
	Handler<Object> result;
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time2 = donut->nowTime();
	{
		result = emachine->start( donut->parse("Global.test2 = 10;", "<MEM>", 0) );
		ASSERT_EQ(10, result->toInt(heap));
		ASSERT_EQ(10, heap->loadGlobalObject("test2")->toInt(heap));
	}
	unsigned int const time3 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time4 = donut->nowTime();

	ASSERT_LT(time1, time2);
	ASSERT_LT(time2, time3);
	ASSERT_LT(time3, time4);

	{
		donut->seek( time1 );
		ASSERT_FALSE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
	}
	{
		donut->seek( time2 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap) );
	}
	{
		donut->seek( time3 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_TRUE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_EQ(10, heap->loadGlobalObject("test2")->toInt(heap) );
	}
	{
		donut->seek( time4 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_TRUE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 2, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_EQ(10, heap->loadGlobalObject("test2")->toInt(heap) );
	}
}

TEST(SeekTest, SeekWithMultMachineAndInterruptTest)
{
	INIT_DONUT;
	Handler<Machine> emachine = donut->queryMachine("another");
	Handler<Object> result;
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time2 = donut->nowTime();
	{
		result = emachine->start( donut->parse("Global.test2 = interrupt 10;", "<MEM>", 0) );
		ASSERT_EQ(10, result->toInt(heap));
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_TRUE( emachine->isInterrupted() );
	}
	unsigned int const time3 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time4 = donut->nowTime();
	{
		result = emachine->startContinue( heap->createInt(20) );
		ASSERT_EQ(20, result->toInt(heap));
		ASSERT_EQ(20, heap->loadGlobalObject("test2")->toInt(heap));
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	unsigned int const time5 = donut->nowTime();

	ASSERT_LT(time1, time2);
	ASSERT_LT(time2, time3);
	ASSERT_LT(time3, time4);
	ASSERT_LT(time4, time5);

	{
		donut->seek( time1 );
		ASSERT_FALSE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_FALSE(  machine->isInterrupted() );
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	{
		donut->seek( time2 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_FALSE(  machine->isInterrupted() );
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	{
		donut->seek( time3 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_FALSE( machine->isInterrupted() );
		ASSERT_TRUE( emachine->isInterrupted() );
	}
	{
		donut->seek( time4 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 2, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_FALSE( machine->isInterrupted() );
		ASSERT_TRUE( emachine->isInterrupted() );
	}
	{
		donut->seek( time5 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_TRUE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 2, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_EQ(20, heap->loadGlobalObject("test2")->toInt(heap) );
		ASSERT_FALSE(  machine->isInterrupted() );
		ASSERT_FALSE( emachine->isInterrupted() );
	}
}

TEST(SeekTest, SeekWithMultMachineAndInterruptAndExecuteTest)
{
	INIT_DONUT;
	Handler<Machine> emachine = donut->queryMachine("another");
	Handler<Object> result;
	unsigned int const time1 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time2 = donut->nowTime();
	{
		result = emachine->start( donut->parse("Global.test2 = interrupt 10;", "<MEM>", 0) );
		ASSERT_EQ(10, result->toInt(heap));
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_TRUE( emachine->isInterrupted() );
	}
	unsigned int const time3 = donut->nowTime();
	{
		result = machine->start( donut->parse("Global.test = 2;", "<MEM>", 0) );
		ASSERT_EQ(2, result->toInt(heap));
		ASSERT_EQ(2, heap->loadGlobalObject("test")->toInt(heap));
	}
	unsigned int const time4 = donut->nowTime();
	{
		result = emachine->startContinue( heap->createInt(20) );
		ASSERT_EQ(20, result->toInt(heap));
		ASSERT_EQ(20, heap->loadGlobalObject("test2")->toInt(heap));
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	unsigned int const time5 = donut->nowTime();

	ASSERT_LT(time1, time2);
	ASSERT_LT(time2, time3);
	ASSERT_LT(time3, time4);
	ASSERT_LT(time4, time5);

	{
		donut->seek( time1 );
		ASSERT_FALSE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_FALSE(  machine->isInterrupted() );
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	{
		donut->seek( time2 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ(1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_FALSE(  machine->isInterrupted() );
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	{
		donut->seek( time3 );
		ASSERT_TRUE( heap->hasGlobalObject("test") );
		ASSERT_FALSE( heap->hasGlobalObject("test2") );
		ASSERT_EQ( 1, heap->loadGlobalObject("test")->toInt(heap) );
		ASSERT_FALSE( machine->isInterrupted() );
		ASSERT_TRUE( emachine->isInterrupted() );
	}
	//分岐
	{
		result = emachine->startContinue( heap->createInt(30) );
		ASSERT_EQ(30, result->toInt(heap));
		ASSERT_TRUE( heap->hasGlobalObject("test2") );
		ASSERT_EQ(30, result->toInt(heap));
		ASSERT_EQ(30, heap->loadGlobalObject("test2")->toInt(heap));
		ASSERT_FALSE( emachine->isInterrupted() );
	}
	unsigned int const time42 = donut->nowTime();
	ASSERT_LT(time3, time42);
	ASSERT_TRUE( heap->hasGlobalObject("test") );
	ASSERT_TRUE( heap->hasGlobalObject("test2") );
	ASSERT_EQ( 1, heap->loadGlobalObject("test")->toInt(heap) );

}

}}


