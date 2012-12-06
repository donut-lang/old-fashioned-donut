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
	std::shared_ptr<Param> p;
	std::string str;
	p = Param::createParam("name", "string", "hi");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_TRUE(p->queryString(&str));
	ASSERT_EQ(str, "hi");
	p = Param::createParam("name", "STRING", "hi");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_TRUE(p->queryString(&str));
	ASSERT_EQ(str, "hi");
}

TEST(ParamTest, IntTest)
{
	std::shared_ptr<Param> p;
	int val = 0xffffffff;
	p = Param::createParam("name", "int", "hi");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_FALSE(p->queryInt(&val));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_EQ(val, 0xffffffff);
	p = Param::createParam("name", "int", "100");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_TRUE(p->queryInt(&val));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_EQ(val, 100);
}

TEST(ParamTest, IntMaxTest)
{
	std::shared_ptr<Param> p;
	{
		int val = 0xFFFFFFFF;
		p = Param::createParam("name", "int", "hi");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(&val));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_EQ(val, 0xFFFFFFFF);
	}

	{
		int val = 0x7FFFFFFF;
		p = Param::createParam("name", "int", "hi");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(&val));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_EQ(val, 0x7FFFFFFF);
	}
}

TEST(ParamTest, FloatTest)
{
	std::shared_ptr<Param> p;
	float val = 0xDEADBEEF;
	p = Param::createParam("name", "float", "float");
	ASSERT_FALSE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_FLOAT_EQ(val, 0xDEADBEEF);

	p = Param::createParam("name", "float", "10.1");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_FLOAT_EQ(val, 10.1);

	p = Param::createParam("name", "float", "nan");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_TRUE(isnan(val));

	p = Param::createParam("name", "float", "NAN");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_TRUE(isnan(val));
}

TEST(ParamTest, BooleanTest)
{
	std::shared_ptr<Param> p;

	{
		bool v=true;
		p = Param::createParam("name", "bool", "float");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(0));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_FALSE(p->queryBool(&v));
		ASSERT_TRUE(v);
	}
	{
		bool v=true;
		p = Param::createParam("name", "bool", "false");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(0));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_TRUE(p->queryBool(&v));
		ASSERT_FALSE(v);
	}
	{
		bool v=true;
		p = Param::createParam("name", "bool", "no");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(0));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_TRUE(p->queryBool(&v));
		ASSERT_FALSE(v);
	}
	{
		bool v=false;
		p = Param::createParam("name", "bool", "true");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(0));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_TRUE(p->queryBool(&v));
		ASSERT_TRUE(v);
	}
	{
		bool v=false;
		p = Param::createParam("name", "bool", "yes");
		ASSERT_FALSE(p->queryFloat(0));
		ASSERT_FALSE(p->queryInt(0));
		ASSERT_FALSE(p->queryString(0));
		ASSERT_TRUE(p->queryBool(&v));
		ASSERT_TRUE(v);
	}
}

TEST(ParamTest, TreeFloatTest)
{
	std::shared_ptr<ParamSet> pset(new ParamSet());
	pset->add("intval", "int", "0");
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pElem;
	pElem = doc.NewElement("widget");
	doc.InsertEndChild(pElem);

	pElem = doc.NewElement("param");
	pElem->SetAttribute("name", "str");
	pElem->InsertFirstChild(doc.NewText("strvalue"));
	doc.RootElement()->InsertEndChild(pElem);

	pElem = doc.NewElement("param");
	pElem->SetAttribute("name", "intval");
	pElem->SetAttribute("type", Param::TypeName::Integer);
	pElem->InsertFirstChild(doc.NewText("256"));
	doc.RootElement()->InsertEndChild(pElem);

	pset->parseTree(doc.RootElement());

	ASSERT_EQ(2, pset->size());
	ASSERT_TRUE(pset->get("intval") != nullptr);
	int val = 0;
	ASSERT_TRUE(pset->get("intval")->queryInt(&val));
	ASSERT_EQ(val, 0);

	std::string str;
	ASSERT_TRUE(pset->get("str") != nullptr);
	ASSERT_TRUE(pset->get("str")->queryString(&str));
	ASSERT_EQ(str, "strvalue");
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
