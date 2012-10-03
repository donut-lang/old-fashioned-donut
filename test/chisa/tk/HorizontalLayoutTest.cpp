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


#include <gtest/gtest.h>
#include <memory>
#include "../../TestCommon.h"
#include "../../../src/chisa/tk/layoyt/HorizontalLayout.h"
#include "../../../src/chisa/tk/layoyt/LayoutFactory.h"

namespace chisa {
namespace tk {
using namespace layout;
using namespace std;
const string fname = "memory";
class HorizontalLayoutTest : public ::testing::Test
{
protected:
	XMLDocument doc;
public:
	void SetUp(){
	}
	void TearDown(){
	}
};

static XMLElement* addHorizontalChild(XMLDocument& doc, XMLElement* horizontal)
{

}

static XMLElement* newHorizontalChild(XMLDocument& doc)
{
	XMLElement* elem = doc.NewElement("horizontal");
	return elem;
}

TEST_F(HorizontalLayoutTest, EmptyTest)
{
	XMLElement* hor = newHorizontalChild(doc);
	doc.InsertFirstChild(hor);
	LayoutFactory factory(log_trace, weak_ptr<World>(), fname, &doc, false);
	shared_ptr<Layout> root = factory.parseTree();
	ASSERT_EQ(root->getChildCount(), 0);
	ASSERT_TRUE(util::startsWith(root->toString(), "(HorizontalLayout"));
}

TEST_F(HorizontalLayoutTest, BasicLayoutTest)
{

}

}}
