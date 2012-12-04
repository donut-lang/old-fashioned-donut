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

TEST(InterruptTest, WithGlobalTest)
{
	std::string src(R"delimiter(
Global.x=0;
Global.x = interrupt 10;
Global.x = interrupt 20;
)delimiter");

	INIT_DONUT;
	ASSERT_FALSE(machine->isInterrupted());
	Handler<Object> result;
	Handler<Source> src_(donut->parse(src));
	{
		result = machine->start( src_ );
		ASSERT_TRUE(machine->isInterrupted());
		ASSERT_EQ(10, result->toInt(heap) );
		ASSERT_EQ( 0, heap->loadGlobalObject("x")->toInt(heap) );
	}
	ASSERT_ANY_THROW( machine->start(src_) );
	{
		result = machine->startContinue( heap->createInt(1) );
		ASSERT_TRUE(machine->isInterrupted());
		ASSERT_EQ(20, result->toInt(heap) );
		ASSERT_EQ( 1, heap->loadGlobalObject("x")->toInt(heap) );
	}
	{
		result = machine->startContinue( heap->createInt(2) );
		ASSERT_FALSE(machine->isInterrupted());
		ASSERT_EQ( 2, result->toInt(heap) );
		ASSERT_EQ( 2, heap->loadGlobalObject("x")->toInt(heap) );
	}
	ASSERT_ANY_THROW( machine->startContinue( heap->createNull() ) );
}


}}


