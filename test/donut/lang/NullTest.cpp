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

TEST(NullTest, EmptyTest)
{
	EXECUTE_SRC("");
	ASSERT_TRUE(result->isNull());
	ASSERT_FALSE(result->isObject());
}

TEST(NullTest, ImplicitConvertTest)
{
	INIT_DONUT;
	ASSERT_ANY_THROW( heap->createNull()->toBool(heap) );
	ASSERT_ANY_THROW( heap->createNull()->toFloat(heap) );
	ASSERT_ANY_THROW( heap->createNull()->toInt(heap) );
	ASSERT_ANY_THROW( heap->createNull()->toString(heap) );
}

TEST(DonutNullTest, LiteralTest)
{
	SOURCE_TEST_NULL("null;");
}

}
