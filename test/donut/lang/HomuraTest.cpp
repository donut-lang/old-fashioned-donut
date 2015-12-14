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

#include "DonutHelper.hpp"
#include <math.h>

namespace donut {

TEST(DonutHomuraTest, NowTest)
{
	INIT_DONUT;
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.now();", "<MEM>", 0) );
		ASSERT_EQ(2+time, result->toInt(heap));
	}
}
TEST(DonutHomuraTest, TickTest)
{
	INIT_DONUT;
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.tick();", "<MEM>", 0) );
		ASSERT_EQ(3+time, donut->nowTime());
		ASSERT_EQ(2+time, result->toInt(heap));
	}
}
TEST(DonutHomuraTest, MemorizeTest)
{
	INIT_DONUT;
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.test=1;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
	}
	donut->seek(time);
	{
		result = machine->start( donut->parse("Homura.test;", "<MEM>", 0) );
		ASSERT_EQ(1, result->toInt(heap));
	}
}

TEST(DonutHomuraTest, LoopTest)
{
	INIT_DONUT;
	{
		Handler<Object> result;
		result = machine->start( donut->parse("Homura.test=1;var t=Homura.tick();if(Homura.test < 10){ Homura.test+=1; Homura.seek(t); }else{ Homura.test; };Homura.test;", "<MEM>", 0) );
		ASSERT_EQ(10, result->toInt(heap));
	}
}

TEST(DonutHomuraTest, BackWithInterruptTest)
{
	INIT_DONUT;
	{
		Handler<Object> result;
		ASSERT_NO_THROW(
				result = machine->start( donut->parse("var first = Homura.now(); var x=interrupt null; if(x){ \"back\"; Homura.seek(first); }else{ \"through\"; };", "<MEM>", 0) ) );
		ASSERT_TRUE(result->isNull());
		ASSERT_TRUE(machine->isInterrupted());

		result = machine->resume(heap->createBool(true));
		ASSERT_TRUE(result->isNull());
		ASSERT_TRUE(machine->isInterrupted());

		result = machine->resume(heap->createBool(false));
		ASSERT_TRUE(result->isObject());
		ASSERT_EQ("through", result->toString(heap));
		ASSERT_FALSE(machine->isInterrupted());
	}
}

TEST(DonutHomuraTest, BackToEntrypointTest)
{
	INIT_DONUT;
	{
		Handler<Object> result;
		ASSERT_NO_THROW(
				result = machine->start( donut->parse("var first = Homura.now(); var x = interrupt null; if(x){ \"back\"; Homura.seek(first-1); }else{ \"through\"; };", "<MEM>", 0) ) );
		ASSERT_TRUE(result->isNull());
		ASSERT_TRUE(machine->isInterrupted());

		ASSERT_NO_THROW( result = machine->resume(heap->createBool(true)) );
		ASSERT_TRUE(result->isNull());
		ASSERT_TRUE(machine->isInterrupted());

		ASSERT_NO_THROW( result = machine->resume(heap->createBool(false)) );
		ASSERT_TRUE(result->isObject());
		ASSERT_EQ("through", result->toString(heap));
		ASSERT_FALSE(machine->isInterrupted());
	}
}

TEST(DonutHomuraTest, SaveTest)
{
	XValue v;
	{
		INIT_DONUT;
		Handler<Object> result;
		result = machine->start( donut->parse("Homura.test=1;", "<MEM>", 0) );
		v = donut->save();
	}
	{
		INIT_DONUT_WITH(v);
		Handler<Object> result;
		ASSERT_NO_THROW( result = machine->start( donut->parse("Homura.test;", "<MEM>", 0) ) );
		ASSERT_EQ(1, result->toInt(heap));
	}
}

}
