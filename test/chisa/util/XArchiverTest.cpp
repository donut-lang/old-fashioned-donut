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

#include "../../TestCommon.h"
#include "../../../src/chisa/util/XArchiver.h"
#include <math.h>
using namespace chisa::util;

namespace chisa {
namespace util {
namespace {

struct BasicTest{
	int x,y,z;
	void serialize(XArchiver& arc){
		arc & x & y & z;
	}
};

struct ComplexTest{
	int x,y,z;
	BasicTest inner;
	void serialize(XArchiver& arc){
		arc & x & y & z & inner;
	}
};

TEST(XArchiveTest, BasicStructTest)
{
	XValue v;
	{
		BasicTest t;
		t.x=1;
		t.y=2;
		t.z=3;
		XArchiverOut arc;
		arc << t;
		arc >> v;
	}
	{
		BasicTest t;
		t.x=0;
		t.y=0;
		t.z=0;
		XArchiverIn arc(v);
		arc >> t;
		ASSERT_EQ(t.x, 1);
		ASSERT_EQ(t.y, 2);
		ASSERT_EQ(t.z, 3);
	}
}

TEST(XArchiveTest, ComplexStructTest)
{
	XValue v;
	{
		ComplexTest t;
		t.x=1;
		t.y=2;
		t.z=3;
		t.inner.x=4;
		t.inner.y=5;
		t.inner.z=6;
		XArchiverOut arc;
		arc << t;
		arc >> v;
		ASSERT_ANY_THROW(arc >> v); //二回同じ操作はできない
	}
	{
		ComplexTest t;
		t.x=0;
		t.y=0;
		t.z=0;
		XArchiverIn arc(v);
		arc >> t;
		ASSERT_ANY_THROW(arc >> t); //二回同じ操作はできない
		ASSERT_EQ(t.x, 1);
		ASSERT_EQ(t.y, 2);
		ASSERT_EQ(t.z, 3);
		ASSERT_EQ(t.inner.x, 4);
		ASSERT_EQ(t.inner.y, 5);
		ASSERT_EQ(t.inner.z, 6);
	}
}

}}}
