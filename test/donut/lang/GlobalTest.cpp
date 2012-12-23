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

TEST(GlobalTest, GlobalExistsTest)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("Global;", "<MEM>", 0) ) );
	ASSERT_TRUE( result->isObject() );
}

TEST(GlobalTest, GlobalSetTest)
{
	INIT_DONUT
	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("Global.x=1;", "<MEM>", 0) ) );
	ASSERT_TRUE( result->isInt() );
	ASSERT_EQ(1, result->toInt(heap) );

	ASSERT_NO_THROW( result = machine->start( donut->parse("Global.x;", "<MEM>", 0) ) );
	ASSERT_TRUE( result->isInt() );
	ASSERT_EQ(1, result->toInt(heap) );
}

TEST(GlobalTest, GlobalScopeTest)
{
	//グローバルに設定した値は、Globalの修飾なしで見えなければならない
	INIT_DONUT
	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("Global.x=1; x;", "<MEM>", 0) ) );
	ASSERT_TRUE( result->isInt() );
	ASSERT_EQ(1, result->toInt(heap) );
}

TEST(GlobalTest, GlobalRegisterTest)
{
	INIT_DONUT
	heap->setGlobalObject( "x", heap->createInt( 1 ) );
	ASSERT_TRUE( heap->hasGlobalObject( "x" ) );
	Handler<Object> result = heap->getGlobalObject( "x" );
	ASSERT_TRUE( result->isInt() );
	ASSERT_EQ(1, result->toInt(heap) );
}

TEST(GlobalTest, GlobalFromScriptTest)
{
	INIT_DONUT
	heap->setGlobalObject( "x", heap->createInt( 1 ) );

	Handler<Object> result;
	ASSERT_NO_THROW( result = machine->start( donut->parse("Global.x;", "<MEM>", 0) ) );
	ASSERT_TRUE( result->isInt() );
	ASSERT_EQ(1, result->toInt(heap) );
}

}
