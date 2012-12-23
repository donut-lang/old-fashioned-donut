/**
 * Saccubus
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

#include "../TestCommon.h"
#include <tarte/ClassUtil.h>

namespace tarte {

class Sample {
	DEFINE_MEMBER(public, public, int, val)
	DEFINE_MEMBER(public, public, std::string, str)
public:
	Sample(int v, std::string const& str):val_(v), str_(str){}
};

TEST(ClassUtilTest, DefineMemberLiteralGetterSetterTest)
{
	Sample s(0,"");
	ASSERT_EQ(0, s.val());
	s.val(0);
	ASSERT_EQ(0, s.val());
	s.val(1);
	ASSERT_EQ(1, s.val());
}

TEST(ClassUtilTest, ConstDefineMemberLiteralGetterSetterTest)
{
	const Sample s(012,"");
	ASSERT_EQ(012, s.val());
}

TEST(ClassUtilTest, DefineMemberLiteralReferenceTest)
{
	Sample s(0,"");
	ASSERT_EQ(0, s.val());
	static_assert(std::is_same<decltype(s.val()), int const&>::value, "oops. overload failed.");
}

TEST(ClassUtilTest, ConstDefineMemberLiteralReferenceTest)
{
	const Sample s(0,"");
	ASSERT_EQ(0, s.val());
	static_assert(std::is_same<decltype(s.val()), int const&>::value, "oops. overload failed.");
}

}
