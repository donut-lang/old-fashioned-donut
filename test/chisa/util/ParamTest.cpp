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
	int val = 0xDEADBEEF;
	p = Param::createParam("name", "int", "hi");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_FALSE(p->queryInt(&val));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_EQ(val, 0xDEADBEEF);
	p = Param::createParam("name", "int", "100");
	ASSERT_FALSE(p->queryFloat(0));
	ASSERT_TRUE(p->queryInt(&val));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_EQ(val, 100);
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

	p = Param::createParam("name", "Float", "10.1");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_FLOAT_EQ(val, 10.1);

	p = Param::createParam("name", "Float", "nan");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_TRUE(isnan(val));

	p = Param::createParam("name", "Float", "NAN");
	ASSERT_TRUE(p->queryFloat(&val));
	ASSERT_FALSE(p->queryInt(0));
	ASSERT_FALSE(p->queryString(0));
	ASSERT_TRUE(isnan(val));
}

TEST(TreeTest, FloatTest)
{
	std::shared_ptr<ParamSet> pset(new ParamSet());
	pset->add("intval", "int", "0");
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pElem;
	pElem = doc.NewElement("widget");
	doc.InsertEndChild(pElem);

	pElem = doc.NewElement("param");
	pElem->SetAttribute("name", "str");
	pElem->SetAttribute("value", "strvalue");
	doc.RootElement()->InsertEndChild(pElem);

	pElem = doc.NewElement("param");
	pElem->SetAttribute("name", "intval");
	pElem->SetAttribute("type", Param::TypeString::Integer);
	pElem->SetAttribute("value", "256");
	doc.RootElement()->InsertEndChild(pElem);

	ParamSet::parseTree(pset, doc.RootElement());

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

}}
