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
		XValue p(XValue::fromString<XSInt>("0x7fffffff"));
		ASSERT_TRUE( p.is<XSInt>() );
		ASSERT_ANY_THROW( p.get<XUInt>() );
		ASSERT_EQ(0x7fffffff, p.get<XSInt>());
	}

	{
		XValue p(XValue::fromString<XUInt>("0xffffffff"));
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
		XValue p(XValue::fromString<XFloat>("nan"));
		ASSERT_TRUE( p.is<XFloat>() );
		ASSERT_TRUE( isnan( p.get<XFloat>() ) );
	}
	{
		XValue p(XValue::fromString<XFloat>("NAN"));
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
		ASSERT_ANY_THROW( p = XValue::fromString<XBool>("float") );
	}
	{
		XValue p( XValue::fromString<XBool>("false") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.get<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("no") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_FALSE( p.get<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("true") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.get<XBool>() );
	}
	{
		XValue p( XValue::fromString<XBool>("yes") );
		ASSERT_TRUE( p.is<XBool>() );
		ASSERT_TRUE( p.get<XBool>() );
	}
}

TEST(ParamTest, TreeTest)
{
	auto tree = parse(R"delimiter(
		<?xml version="1.0" encoding="UTF-8"?>
		<xobject>
			<xpair name="a"><xbool>true</xbool></xpair>
			<xpair name="b"><xsint>-1</xsint></xpair>
			<xpair name="c"><xuint>1</xuint></xpair>
			<xpair name="d"><xstring>test</xstring></xpair>
			<xpair name="e"><xfloat>10.11</xfloat></xpair>
			<xpair name="f">
				<xarray>
					<xsint>10</xsint>
					<xsint>20</xsint>
				</xarray>
			</xpair>
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
	ParamSet p;
	p.addInt("int", 1);
	p.addFloat("float", 1.2f);
	p.addString("str", "text");
	p.addBool("bool", true);

	ASSERT_EQ(1, p.getInt("int"));
	ASSERT_FLOAT_EQ(1.2f, p.getFloat("float"));
	ASSERT_EQ("text", p.getString("str"));
	ASSERT_EQ(true, p.getBool("bool"));
}

TEST(ParamTest, SerializeDeserializeTest)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* elm;
	{
		ParamSet p;
		p.addInt("int", 0xFFFFFFFF);
		p.addInt("int2", 0x7FFFFFFF);
		p.addFloat("float", 1.2f);
		p.addString("str", "text");
		p.addBool("bool", true);
		elm = p.synthTree(&doc);
		//こうしないとリークする
		doc.InsertFirstChild(elm);
	}

	{
		ParamSet p;
		p.parseTree(elm);
		ASSERT_EQ(0xFFFFFFFF, p.getInt("int"));
		ASSERT_EQ(0x7FFFFFFF, p.getInt("int2"));
		ASSERT_FLOAT_EQ(1.2f, p.getFloat("float"));
		ASSERT_EQ("text", p.getString("str"));
		ASSERT_EQ(true, p.getBool("bool"));
	}
}


}}
