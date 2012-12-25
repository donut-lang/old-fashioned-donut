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
#include <tarte/XVal.h>
#include <math.h>

namespace tarte {

TEST(XValueTest, SizeTest)
{
	//static_debug<sizeof(XValue), sizeof(void*), sizeof(XValue::Type)>();
	static_assert( sizeof(XValue) <= sizeof(void*)*2, "size of XValue must be less than or equal to that of 2 pointers." );
}

TEST(XValueTest, ArrayHandlerTest)
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

TEST(XValueTest, SwapTest)
{
	XValue x(10);
	XValue y(-1.3f);
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(10, x.as<XSInt>() );
	ASSERT_TRUE( y.is<XFloat>() );
	ASSERT_EQ(-1.3f, y.as<XFloat>() );

	using std::swap;
	swap(x,y);

	ASSERT_TRUE( y.is<XSInt>() );
	ASSERT_EQ(10, y.as<XSInt>() );
	ASSERT_TRUE( x.is<XFloat>() );
	ASSERT_EQ(-1.3f, x.as<XFloat>() );
}

TEST(XValueTest, AssignTest)
{
	XValue x(10);
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(10, x.as<XSInt>() );

	x.as<XSInt>() = 20;
	ASSERT_TRUE( x.is<XSInt>() );
	ASSERT_EQ(20, x.as<XSInt>() );
}
TEST(XValueTest, NullTest)
{
	XValue x;
	ASSERT_TRUE(x.is<XNull>() );
	ASSERT_FALSE( x.is<XSInt>() );
	ASSERT_EQ( nullptr, x.as<XNull>() );
}

TEST(XValueTest, AssignObjectTest)
{
	XValue x(Handler<XObject>(new XObject()));
	ASSERT_TRUE( x.is<XObject>() );

	x.as<XObject>()->set("name", 10);
	ASSERT_TRUE( x.as<XObject>()->has<XSInt>("name") );
	ASSERT_EQ(10, x.as<XObject>()->get<XSInt>("name") );

	x.as<XObject>() = Handler<XObject>(new XObject());
	ASSERT_TRUE( x.is<XObject>() );
	ASSERT_FALSE( x.as<XObject>()->has<XSInt>("name") );
}

TEST(XValueTest, ObjectHandlerTest)
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

TEST(XValueTest, StringTest)
{
	XValue x("string");
	ASSERT_TRUE( x.is<XString>() );
	ASSERT_EQ( "string", x.as<XString>() );
}

TEST(XValueTest, IntTest)
{
	int val = 0xffffffff;
	{
		XValue p((val));
		ASSERT_TRUE( p.is<XSInt>() );
		ASSERT_ANY_THROW( p.as<XUInt>() );
		ASSERT_EQ(p.as<XSInt>(), 0xffffffff);
	}
	{
		XValue p((10U));
		ASSERT_TRUE( p.is<XUInt>() );
		ASSERT_ANY_THROW( p.as<XSInt>() );
		ASSERT_EQ(10, p.as<XUInt>());
	}
}

TEST(XValueTest, IntMaxTest)
{
	{
		XValue p((0xFFFFFFFF));
		ASSERT_EQ(0xFFFFFFFF, p.as<XUInt>());
	}

	{
		XValue p((0x7FFFFFFF));
		ASSERT_EQ(0x7FFFFFFF, p.as<XSInt>());
	}

	{
		XValue p(XValue::fromString<XSInt>("0x7fffffff"));
		ASSERT_TRUE( p.is<XSInt>() );
		ASSERT_ANY_THROW( p.as<XUInt>() );
		ASSERT_EQ(0x7fffffff, p.as<XSInt>());
	}

	{
		XValue p(XValue::fromString<XUInt>("0xffffffff"));
		ASSERT_TRUE( p.is<XUInt>() );
		ASSERT_ANY_THROW( p.as<XSInt>() );
		ASSERT_EQ(0xffffffff, p.as<XUInt>());
	}
}

TEST(XValueTest, FloatTest)
{
	{
		float val = 0xDEADBEEF;
		XValue p((val));
		ASSERT_EQ(val, p.as<XFloat>());
	}
	{
		float val = 10.1;
		XValue p((val));
		ASSERT_EQ(val, p.as<XFloat>());
	}
	{
		float val = NAN;
		XValue p((val));
		ASSERT_TRUE( std::isnan( p.as<XFloat>() ) );
	}
	{
		XValue p(XValue::fromString<XFloat>("nan"));
		ASSERT_TRUE( p.is<XFloat>() );
		ASSERT_TRUE( std::isnan( p.as<XFloat>() ) );
	}
	{
		XValue p(XValue::fromString<XFloat>("NAN"));
		ASSERT_TRUE( p.is<XFloat>() );
		ASSERT_TRUE( std::isnan( p.as<XFloat>() ) );
	}
}

TEST(XValueTest, BinaryTest)
{
	char orig[256];
	for(int i=0;i<256;++i){
		orig[i] = i;
	}
	{
		XValue p(orig, 256);
		XBinary const& b = p.as<XBinary>();
		for(int i=0;i<256;++i){
			ASSERT_EQ(orig[i], b.at(i));
		}
		ASSERT_EQ(std::vector<char>(orig, orig+256), b);
	}
}

TEST(XValueTest, BooleanTest)
{
	{
		bool val = true;
		XValue p((val));
		ASSERT_EQ(val, p.as<XBool>());
	}

	{
		XValue p;
		ASSERT_ANY_THROW( p = XValue::fromString<XBool>("float") );
	}
	{
		XValue p( XValue::fromString<XBool>("false") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.as<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("no") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.as<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("true") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.as<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("yes") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.as<XBool>() );
	}
}

namespace {

enum SampleEnum{
	X,Y,Z
};

TEST(XValueTest, EnumTest)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* e;
	std::string str;
	{
		SampleEnum v = SampleEnum::Y;
		XValue z(v);
		e = z.toXML(&doc);
		doc.InsertEndChild(e);
	}
	{
		XValue z=XValue::fromXML(e);
		ASSERT_TRUE(z.is<SampleEnum>());
		ASSERT_EQ(SampleEnum::Y, z.as<SampleEnum>());
	}
}

}

TEST(XValueTest, TreeTest)
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
	Handler<XObject> obj = p.as<XObject>();
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

TEST(XValueTest, InvalidTreeTest)
{
	auto tree = parse(R"delimiter(
		<?xml version="1.0" encoding="UTF-8"?>
		<xobject_invalid>
		</xobject_invalid>
		)delimiter");
	tree->PrintError();
	tinyxml2::XMLElement* elm = tree->RootElement();
	ASSERT_ANY_THROW( XValue::fromXML(elm) );

	Handler<XObject> obj(new XObject);
	ASSERT_ANY_THROW(obj->accumlate(elm));

}

TEST(XValueTest, ObjectGetSetTest)
{
	Handler<XObject> obj(new XObject());
	obj->set("int", 1);
	obj->set("float", 1.2f);
	obj->set("str", "test");
	obj->set("bool", true);

	ASSERT_TRUE(obj->has<XSInt>("int"));
	ASSERT_EQ(1, obj->get<XSInt>("int"));
	ASSERT_TRUE(obj->has<XFloat>("float"));
	ASSERT_FLOAT_EQ(1.2f, obj->get<XFloat>("float"));
	ASSERT_TRUE(obj->has<XString>("str"));
	ASSERT_EQ("test", obj->get<XString>("str"));
	ASSERT_TRUE(obj->has<XBool>("bool"));
	ASSERT_EQ(true, obj->get<XBool>("bool"));

	obj->set<XUInt>("int", 12U);
	ASSERT_FALSE(obj->has<XSInt>("int"));
	ASSERT_TRUE(obj->has<XUInt>("int"));
	ASSERT_EQ(12, obj->get<XUInt>("int"));
}

TEST(XValueTest, ObjectConversionTest)
{
	// set時には、入った場所への参照を返さなければならない。それを確かめるために代入する
	Handler<XObject> obj(new XObject());
	obj->set("int", 1) = 10;

	ASSERT_TRUE(obj->has<XSInt>("int"));
	ASSERT_EQ(10, obj->get<XSInt>("int"));
}

TEST(XValueTest, ArrayConversionTest)
{
	// set時には、入った場所への参照を返さなければならない。それを確かめるために代入する
	Handler<XArray> obj(new XArray());
	obj->append(1) = 10;

	ASSERT_TRUE(obj->has<XSInt>(0));
	ASSERT_EQ(10, obj->get<XSInt>(0));

	obj->set(0, "string") = "foo";
	ASSERT_TRUE(obj->has<XString>(0));
	ASSERT_EQ("foo", obj->get<XString>(0));
}
TEST(XValueTest,ObjectNotFoundOrMismatchedTypeTest)
{
	Handler<XObject> obj(new XObject());
	obj->set("int", 1);
	obj->set("float", 1.2f);
	obj->set("str", "test");
	obj->set("bool", true);

	ASSERT_FALSE(obj->has<XSInt>("int2"));
	ASSERT_ANY_THROW(obj->get<XSInt>("int2"));
	ASSERT_FALSE(obj->has<XFloat>("float2"));
	ASSERT_ANY_THROW( obj->get<XFloat>("float2"));
	ASSERT_FALSE(obj->has<XString>("str2"));
	ASSERT_ANY_THROW(obj->get<XString>("str2"));
	ASSERT_FALSE(obj->has<XBool>("bool2"));
	ASSERT_ANY_THROW(obj->get<XBool>("bool2"));

	ASSERT_FALSE(obj->has<XFloat>("int"));
	ASSERT_ANY_THROW(obj->get<XFloat>("int"));
	obj.reset();
}

TEST(XValueTest,ArrayGetSetTest)
{
	Handler<XArray> obj(new XArray);
	obj->append(1);
	obj->append(1.2f);
	obj->append("test");
	obj->append(true);

	ASSERT_EQ(4, obj->size());
	ASSERT_EQ(1, obj->get<XSInt>(0));
	ASSERT_FLOAT_EQ(1.2f,obj->get<XFloat>(1));
	ASSERT_EQ("test", obj->get<XString>(2));
	ASSERT_TRUE(obj->get<XBool>(3));
	obj.reset();
}

TEST(XValueTest,ArrayUpdateTest)
{
	Handler<XArray> obj(new XArray);
	obj->append(1);
	ASSERT_EQ(1, obj->size());
	ASSERT_EQ(1, obj->get<XSInt>(0));

	obj->set(0, std::string("test"));
	ASSERT_EQ(1, obj->size());
	ASSERT_ANY_THROW(obj->get<XSInt>(0));
	ASSERT_EQ("test", obj->get<XString>(0));
	obj.reset();
}

TEST(XValueTest,ArrayInvalidIndexOrMismatchedTypeTest)
{
	Handler<XArray> obj(new XArray);
	obj->append(1);
	obj->append(1.2f);
	obj->append("test");
	obj->append(true);

	ASSERT_ANY_THROW(obj->get<XSInt>(-1));
	ASSERT_ANY_THROW(obj->get<XSInt>(4));
	ASSERT_ANY_THROW(obj->get<XSInt>(100));

	ASSERT_ANY_THROW(obj->get<XUInt>(0));
	ASSERT_ANY_THROW(obj->get<XSInt>(1));
	ASSERT_ANY_THROW(Handler<XArray> a = obj->get<XArray>(2));
	ASSERT_ANY_THROW(Handler<XObject> a = obj->get<XObject>(3));
	obj.reset();
}

TEST(XValueTest, SerializeDeserializeTest)
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


}

