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
#include "../../../src/chisa/util/Param.h"
#include <math.h>
using namespace chisa::util;

namespace chisa {
namespace util {

TEST(ParamTest, SizeTest)
{
	//static_debug<sizeof(XValue), sizeof(void*), sizeof(XValue::Type)>();
	static_assert( sizeof(XValue) <= sizeof(void*)*2, "size of XValue must be less than or equal to that of 2 pointers." );
}

TEST(ParamTest, ArrayHandlerTest)
{
	Handler<XArray> array( new XArray() );
	{
		XValue x (array);
		//assign refs
		XValue y (10);
		ASSERT_EQ(2, array.refcount());
		x=y;
		y=x;
		// rvalue
		x = XValue(x);
		x = XValue(10.1f);
		// Effective STL-6 参照　関数宣言と間違えられないために、余分にカッコをつける。
		XValue z((XValue(XValue(x))));

		//swap
		using std::swap;
		swap(x,y);
		swap(y,z);
		swap(z,x);
	}
	ASSERT_EQ(1, array.refcount());
}

TEST(ParamTest, SwapTest)
{
	XValue x(10);
	XValue y(-1.3f);
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(10, x.get<XSInt>() );
	ASSERT_TRUE( y.is<XFloat>() );
	ASSERT_EQ(-1.3f, y.get<XFloat>() );

	using std::swap;
	swap(x,y);

	ASSERT_TRUE( y.is<XSInt>() );
	ASSERT_EQ(10, y.get<XSInt>() );
	ASSERT_TRUE( x.is<XFloat>() );
	ASSERT_EQ(-1.3f, x.get<XFloat>() );
}

TEST(ParamTest, AssignTest)
{
	XValue x(10);
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(10, x.get<XSInt>() );

	x.get<XSInt>() = 20;
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(20, x.get<XSInt>() );
}

TEST(ParamTest, AssignObjectTest)
{
	XValue x(Handler<XObject>(new XObject()));
	ASSERT_TRUE( x.is<XObject>() );

	x.get<XObject>()->set("name", 10);
	ASSERT_TRUE( x.get<XObject>()->has<XSInt>("name") );
	ASSERT_EQ(10, x.get<XObject>()->get<XSInt>("name") );

	x.get<XObject>() = Handler<XObject>(new XObject());
	ASSERT_TRUE( x.is<XObject>() );
	ASSERT_FALSE( x.get<XObject>()->has<XSInt>("name") );
}

TEST(ParamTest, ObjectHandlerTest)
{
	Handler<XObject> obj( new XObject() );
	{
		XValue x (obj);
		//assign refs
		XValue y (10);
		ASSERT_EQ(2, obj.refcount());
		x=y;
		y=x;
		// rvalue
		x = XValue(x);
		x = XValue(10.1f);
		// Effective STL-6 参照　関数宣言と間違えられないために。
		XValue z((XValue(XValue(x))));

		//swap
		using std::swap;
		swap(x,y);
		swap(y,z);
		swap(z,x);
	}
	ASSERT_EQ(1, obj.refcount());
}

TEST(ParamTest, StringTest)
{
	XValue x("string");
	ASSERT_TRUE( x.is<XString>() );
	ASSERT_EQ( "string", x.get<XString>() );
}

TEST(ParamTest, IntTest)
{
	int val = 0xffffffff;
	{
		XValue p((val));
		ASSERT_TRUE( p.is<XSInt>() );
		ASSERT_ANY_THROW( p.get<XUInt>() );
		ASSERT_EQ(p.get<XSInt>(), 0xffffffff);
	}
	{
		XValue p((10U));
		ASSERT_TRUE( p.is<XUInt>() );
		ASSERT_ANY_THROW( p.get<XSInt>() );
		ASSERT_EQ(10, p.get<XUInt>());
	}
}

TEST(ParamTest, IntMaxTest)
{
	std::shared_ptr<Param> p;
	{
		XValue p((0xFFFFFFFF));
		ASSERT_EQ(0xFFFFFFFF, p.get<XUInt>());
	}

	{
		XValue p((0x7FFFFFFF));
		ASSERT_EQ(0x7FFFFFFF, p.get<XSInt>());
	}

	{
		XValue p(XValue::decode<XSInt>("0x7fffffff"));
		ASSERT_TRUE( p.is<XSInt>() );
		ASSERT_ANY_THROW( p.get<XUInt>() );
		ASSERT_EQ(0x7fffffff, p.get<XSInt>());
	}

	{
		XValue p(XValue::decode<XUInt>("0xffffffff"));
		ASSERT_TRUE( p.is<XUInt>() );
		ASSERT_ANY_THROW( p.get<XSInt>() );
		ASSERT_EQ(0xffffffff, p.get<XUInt>());
	}
}

TEST(ParamTest, FloatTest)
{
	{
		float val = 0xDEADBEEF;
		XValue p((val));
		ASSERT_EQ(val, p.get<XFloat>());
	}
	{
		float val = 10.1;
		XValue p((val));
		ASSERT_EQ(val, p.get<XFloat>());
	}
	{
		float val = NAN;
		XValue p((val));
		ASSERT_TRUE( isnan( p.get<XFloat>() ) );
	}
	{
		XValue p(XValue::decode<XFloat>("nan"));
		ASSERT_TRUE( p.is<XFloat>() );
		ASSERT_TRUE( isnan( p.get<XFloat>() ) );
	}
	{
		XValue p(XValue::decode<XFloat>("NAN"));
		ASSERT_TRUE( p.is<XFloat>() );
		ASSERT_TRUE( isnan( p.get<XFloat>() ) );
	}
}

TEST(ParamTest, BooleanTest)
{
	{
		bool val = true;
		XValue p((val));
		ASSERT_EQ(val, p.get<XBool>());
	}

	{
		XValue p;
		ASSERT_ANY_THROW( p = XValue::decode<XBool>("float") );
	}
	{
		XValue p( XValue::decode<XBool>("false") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.get<XBool>() );
	}
	{
		XValue p( XValue::decode<XBool>("no") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.get<XBool>() );
	}
	{
		XValue p( XValue::decode<XBool>("true") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.get<XBool>() );
	}
	{
		XValue p( XValue::decode<XBool>("yes") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.get<XBool>() );
	}
}

TEST(ParamTest, TreeTest)
{
	auto tree = parse(R"delimiter(
		<?xml version="1.0" encoding="UTF-8"?>
		<xobject>
			<xbool name="a">true</xbool>
			<xsint name="b">-1</xsint>
			<xuint name="c">1</xuint>
			<xstring name="d">test</xstring>
			<xfloat name="e">10.11</xfloat>
			<xarray name="f">
				<xsint>10</xsint>
				<xsint>20</xsint>
			</xarray>
		</xobject>
		)delimiter");
	tree->PrintError();
	tinyxml2::XMLElement* elm = tree->RootElement();
	XValue p( XValue::fromXML( elm ) );
	ASSERT_TRUE( p.is<XObject>() );
	Handler<XObject> obj = p.get<XObject>();
	{
		ASSERT_TRUE(obj->has<XBool>("a"));
		ASSERT_TRUE(obj->get<XBool>("a"));

		ASSERT_TRUE(obj->has<XSInt>("b"));
		ASSERT_EQ(-1, obj->get<XSInt>("b"));

		ASSERT_TRUE(obj->has<XUInt>("c"));
		ASSERT_EQ(1, obj->get<XUInt>("c"));

		ASSERT_TRUE(obj->has<XString>("d"));
		ASSERT_EQ("test", obj->get<XString>("d"));

		ASSERT_TRUE(obj->has<XFloat>("e"));
		ASSERT_FLOAT_EQ(10.11, obj->get<XFloat>("e"));

		ASSERT_TRUE(obj->has<XArray>("f"));
		Handler<XArray> array(obj->get<XArray>("f"));
		{
			ASSERT_EQ(2, array->size());
			ASSERT_EQ(10, array->get<XSInt>(0));
			ASSERT_EQ(20, array->get<XSInt>(1));
		}
	}
}

TEST(ParamTest, GetSetTest)
{
	Handler<XObject> obj(new XObject());
	obj->set("int", 1);
	obj->set("float", 1.2f);
	obj->set("str", "test");
	obj->set("bool", true);

	ASSERT_EQ(1, obj->get<XSInt>("int"));
	ASSERT_FLOAT_EQ(1.2f, obj->get<XFloat>("float"));
	ASSERT_EQ("test", obj->get<XString>("str"));
	ASSERT_EQ(true, obj->get<XBool>("bool"));
}

TEST(ParamTest, SerializeDeserializeTest)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* elm;
	{
		Handler<XObject> obj(new XObject());
		obj->set("int", 0xFFFFFFFF);
		obj->set("int2", 0x7FFFFFFF);
		obj->set("float", 1.2f);
		obj->set("str", "text");
		obj->set("bool", true);
		elm = obj->toXML(&doc);
		//こうしないとリークする
		doc.InsertFirstChild(elm);
	}

	{
		Handler<XObject> obj(new XObject(elm));
		ASSERT_EQ(0xFFFFFFFF, obj->get<XUInt>("int"));
		ASSERT_EQ(0x7FFFFFFF, obj->get<XSInt>("int2"));
		ASSERT_FLOAT_EQ(1.2f, obj->get<XFloat>("float"));
		ASSERT_EQ("text", obj->get<XString>("str"));
		ASSERT_EQ(true, obj->get<XBool>("bool"));
	}
}


}}
