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

TEST(DonutHomuraTest, NowTest)
{
	INIT_DONUT;
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.now();", "<MEM>", 0) );
		ASSERT_EQ(1+time, result->toInt(heap));
	}
}
TEST(DonutHomuraTest, TickTest)
{
	INIT_DONUT;
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.tick();", "<MEM>", 0) );
		ASSERT_EQ(1+time, result->toInt(heap));
		ASSERT_EQ(2+time, donut->nowTime());
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
	const timestamp_t time = donut->nowTime();
	Handler<Object> result;
	{
		result = machine->start( donut->parse("Homura.test=1;t=Homura.tick();if(Homura.test < 10){ Homura.test+=1; Homura.seek(t); }else{ Homura.test; };Homura.test;", "<MEM>", 0) );
		ASSERT_EQ(10, result->toInt(heap));
	}
}
}}

