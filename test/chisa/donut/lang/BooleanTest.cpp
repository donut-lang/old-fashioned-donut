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

TEST(DonutBoolTest, ImplicitConvertTest)
{
	INIT_DONUT;
	ASSERT_NO_THROW( heap->createBool(true)->toBool(heap) );
	ASSERT_ANY_THROW( heap->createBool(true)->toFloat(heap) );
	ASSERT_ANY_THROW( heap->createBool(true)->toInt(heap) );
	ASSERT_ANY_THROW( heap->createBool(true)->toString(heap) );
}

TEST(DonutBooleanTest, LiteralTest)
{
	SOURCE_TEST_TRUE("true");
	SOURCE_TEST_FALSE("false");
}
TEST(DonutBooleanTest, AndTest)
{
	SOURCE_TEST_FALSE("false && false");
	SOURCE_TEST_FALSE("false && true");
	SOURCE_TEST_FALSE("true && false");
	SOURCE_TEST_TRUE("true && true");
}
TEST(DonutBooleanTest, OrTest)
{
	SOURCE_TEST_TRUE("true || true");
	SOURCE_TEST_TRUE("true || false");
	SOURCE_TEST_TRUE("false || true");
	SOURCE_TEST_FALSE("false || false");
}
TEST(DonutBooleanTest, NotTest)
{
	SOURCE_TEST_FALSE("!true");
	SOURCE_TEST_TRUE("!false");
}

TEST(DonutBooleanTest, ToStringTest)
{
	SOURCE_TEST_STR("true","true.toString();");
	SOURCE_TEST_STR("false","false.toString();");
}


}}


