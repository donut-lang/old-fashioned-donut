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

namespace donut {

TEST(LocalTest, LocalTest)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("x=1;", "<MEM>", 0) ) );
	ASSERT_FALSE( result->isObject() );
	ASSERT_EQ(1, result->toInt(heap) );
	ASSERT_FALSE( heap->hasGlobalObject("x") );
}

TEST(LocalTest, LocalVarNotKeepingTest)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("x=1;", "<MEM>", 0) ) );
	ASSERT_FALSE( result->isObject() );
	ASSERT_EQ(1, result->toInt(heap) );
	ASSERT_ANY_THROW( result = machine->start( donut->parse("x;", "<MEM>", 0) ) );
}

}
