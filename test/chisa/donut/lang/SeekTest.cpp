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
	unsigned int const time1 = donut->nowTime();
	Handler<Object> result = machine->start( donut->parse("test = 1;", "<MEM>", 0) );
	ASSERT_EQ(1, result->toInt(heap));
	unsigned int const time2 = donut->nowTime();

	ASSERT_LT(time1, time2);

	result = machine->start( donut->parse("test = 2;", "<MEM>", 0) );
	ASSERT_EQ(2, result->toInt(heap));
	unsigned int const time3 = donut->nowTime();

	ASSERT_LT(time2, time3);
}

TEST(SeekTest, SeekBefore)
{
	INIT_DONUT
	unsigned int const time1 = donut->nowTime();
	Handler<Object> result = machine->start( donut->parse("test = 1;", "<MEM>", 0) );
	unsigned int const time2 = donut->nowTime();
	result = machine->start( donut->parse("test = 2;", "<MEM>", 0) );
}

}}


