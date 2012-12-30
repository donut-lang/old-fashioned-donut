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

#include "../TestCommon.h"
#include <tarte/Dynamic.h>

namespace tarte {

TEST(DemangleTest, Literal)
{
	int d=2;
	ASSERT_EQ("int", demangle(d));
	ASSERT_EQ("int", demangle(&d));
	ASSERT_EQ("int", demangle<int>());
	ASSERT_EQ("int", demangle<int&>());
	ASSERT_EQ("int", demangle<int&&>());
	ASSERT_EQ("int", demangle<int*>());
	ASSERT_EQ("int", demangle<int**>());
	ASSERT_EQ("int", demangle<int***>());

	float f=0.0f;
	ASSERT_EQ("float", demangle(f));
}

class __DemangleTest__Sample {

};

TEST(DemangleTest, Class)
{
	__DemangleTest__Sample s;
	ASSERT_EQ("tarte::__DemangleTest__Sample", demangle(s));
	ASSERT_EQ("tarte::__DemangleTest__Sample", demangle(&s));
}

template <typename T>
class __DemangleTest__TemplateKlass {

};

TEST(DemangleTest, TemplateClass)
{
	__DemangleTest__TemplateKlass<__DemangleTest__Sample> s;
	ASSERT_EQ("tarte::__DemangleTest__TemplateKlass<tarte::__DemangleTest__Sample>", demangle(s));
}

TEST(DemangleTest, RecusriveTemplateClass)
{
	tarte::__DemangleTest__TemplateKlass<__DemangleTest__TemplateKlass<__DemangleTest__Sample>> s;
	ASSERT_EQ("tarte::__DemangleTest__TemplateKlass<tarte::__DemangleTest__TemplateKlass<__DemangleTest__Sample> >", demangle(s));
}

}

