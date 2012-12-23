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

#include "Helper.h"
#include <math.h>

namespace donut {

class ObjectTest : public ::testing::Test
{
protected:
	Handler<Donut> donut;
	Handler<Clock> clock;
	Handler<Heap> heap;
public:
	void SetUp(){
		donut = Handler<Donut>(new Donut(log_trace));
		donut->bootstrap();
		clock = donut->clock();
		heap = donut->heap();
	}
	void TearDown(){
	}
};

TEST_F(ObjectTest, CastTest)
{
	{
		const object_desc_t val = 10;
		const object_desc_t restored = Object::castToDescriptor ( Object::castToPointer(val) );
		ASSERT_EQ(val, restored);
	}
	{
		const object_desc_t val = (object_desc_t)-1;
		const object_desc_t restored = Object::castToDescriptor ( Object::castToPointer(val) );
		ASSERT_EQ(val, restored);
	}
}

TEST_F(ObjectTest, ObjectIdEncodingTest)
{
	{
		const objectid_t val = 10;
		const objectid_t restored = Object::decodeObjectId( Object::encodeObjectId(val) );
		ASSERT_EQ(val, restored);
	}
	{
		const objectid_t val = (objectid_t)-1;
		const objectid_t restored = Object::decodeObjectId( Object::encodeObjectId(val) );
		ASSERT_EQ(val, restored);
	}
}

TEST_F(ObjectTest, ObjectPrimitiveCheckTest)
{
	ASSERT_TRUE( Object::isPrimitiveDescriptor(heap->createInt(1)->toDescriptor()) );
	ASSERT_TRUE( Object::isPrimitiveDescriptor(heap->createInt(-1)->toDescriptor()) );
	ASSERT_TRUE( Object::isPrimitiveDescriptor(heap->createBool(true)->toDescriptor()) );
	ASSERT_TRUE( Object::isPrimitiveDescriptor(heap->createBool(false)->toDescriptor()) );
	ASSERT_TRUE( Object::isPrimitiveDescriptor(heap->createNull()->toDescriptor()) );

	ASSERT_FALSE( Object::isPrimitiveDescriptor(heap->createDonutObject()->toDescriptor()) );
	ASSERT_FALSE( Object::isPrimitiveDescriptor(heap->createEmptyDonutObject()->toDescriptor()) );
	ASSERT_FALSE( Object::isPrimitiveDescriptor(heap->createFloatObject(10.1)->toDescriptor()) );
	ASSERT_FALSE( Object::isPrimitiveDescriptor(heap->createFloatObject(NAN)->toDescriptor()) );
	ASSERT_FALSE( Object::isPrimitiveDescriptor(heap->createStringObject("str")->toDescriptor()) );
}

}
