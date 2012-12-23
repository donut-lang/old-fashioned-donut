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

TEST(FloatTest, ImplicitConvertTest)
{
	INIT_DONUT;
	ASSERT_ANY_THROW( heap->createFloatObject(10)->toBool(heap) );
	ASSERT_NO_THROW( heap->createFloatObject(10)->toFloat(heap) );
	ASSERT_ANY_THROW( heap->createFloatObject(10)->toInt(heap) );
	ASSERT_ANY_THROW( heap->createFloatObject(10)->toString(heap) );
}

TEST(FloatTest, LiteralTest)
{
	SOURCE_TEST_FLOAT(12.34, "12.34;");
}

TEST(FloatTest, CastTest)
{
	INIT_DONUT;
	Handler<FloatObject> obj( heap->createFloatObject(10.1) );
	ASSERT_ANY_THROW(obj->toBool(heap));
	ASSERT_ANY_THROW(obj->toInt(heap));
	ASSERT_ANY_THROW(obj->toString(heap));
	ASSERT_FLOAT_EQ(10.1, obj->toFloat(heap));
}

TEST(FloatTest, SaveLoadTest)
{
	INIT_DONUT;
	std::string providerName;
	XValue data;
	{
		Handler<FloatObject> obj( heap->createFloatObject(10.1) );
		ASSERT_ANY_THROW(obj->toBool(heap));
		ASSERT_ANY_THROW(obj->toInt(heap));
		ASSERT_ANY_THROW(obj->toString(heap));
		ASSERT_FLOAT_EQ(10.1, obj->toFloat(heap));
		data = Handler<HeapObject>(obj)->save(heap);
		providerName = obj->providerName(heap);
	}
	{
		Handler<HeapObject> obj(heap->findHeapProvider(providerName)->createEmptyObject());
		obj->load(heap, data);
		ASSERT_NO_THROW(obj.cast<FloatObject>());
		ASSERT_ANY_THROW(obj->toBool(heap));
		ASSERT_ANY_THROW(obj->toInt(heap));
		ASSERT_ANY_THROW(obj->toString(heap));
		ASSERT_FLOAT_EQ(10.1, obj->toFloat(heap));
	}
}

TEST(FloatTest, AddTest) { SOURCE_TEST_FLOAT(12.34+123.45, "12.34+123.45;"); }
TEST(FloatTest, SubTest) { SOURCE_TEST_FLOAT(12.34-123.45, "12.34-123.45;"); }
TEST(FloatTest, DivTest) { SOURCE_TEST_FLOAT(12.34/123.45, "12.34/123.45;"); }
TEST(FloatTest, MulTest) { SOURCE_TEST_FLOAT(12.34*123.45, "12.34*123.45;"); }
TEST(FloatTest, LtTest) { SOURCE_TEST_TRUE("12.34<123.45;");SOURCE_TEST_FALSE("12.34<12.34;"); }
TEST(FloatTest, GtTest) { SOURCE_TEST_FALSE("12.34>123.45;");SOURCE_TEST_FALSE("12.34>12.34;"); }
TEST(FloatTest, LeTest) { SOURCE_TEST_TRUE("12.34<=123.45;"); SOURCE_TEST_TRUE("12.34<=12.34;");}
TEST(FloatTest, GeTest) { SOURCE_TEST_FALSE("12.34>=123.45;"); SOURCE_TEST_TRUE("12.34>=12.34;");}
TEST(FloatTest, EqTest) { SOURCE_TEST_TRUE("12.34==12.34;"); }
TEST(FloatTest, NeTest) { SOURCE_TEST_TRUE("12.34!=123.45;"); }
TEST(FloatTest, PlusTest) { SOURCE_TEST_FLOAT(12.34, "+12.34;");SOURCE_TEST_FLOAT(-12.34, "+(-12.34);"); }
TEST(FloatTest, MinusTest) { SOURCE_TEST_FLOAT(-12.34, "-12.34;");SOURCE_TEST_FLOAT(12.34, "-(-12.34);"); }

TEST(FloatTest, IntCastTest)
{
	SOURCE_TEST_INT(12, "(12.34).toInteger();");
}

TEST(FloatTest, ToStringTest)
{
	SOURCE_TEST_STR(toString((10.12345)), "(10.12345).toString();");
}

}
