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
#include "../../src/chisa/tk/layout/SplitLayout.h"
#include "../../src/chisa/tk/layout/LayoutFactory.h"

namespace chisa {
namespace tk {
using namespace layout;
using namespace std;
const string fname = "memory";
class HorizontalLayoutTest : public ::testing::Test
{
protected:
	XMLDocument doc;
	XMLElement* world;
public:
	void SetUp(){
		world = doc.NewElement("world");
		doc.InsertEndChild(world);
	}
	void TearDown(){
	}
};

static XMLElement* addHorizontalChild(XMLDocument& doc, XMLElement* horizontal, const string& elemname, float weight=geom::Unspecified, float min=geom::Unspecified, float max=geom::Unspecified)
{
	XMLElement* elem = doc.NewElement(elemname.c_str());
	elem->SetAttribute("split-weight", weight);
	elem->SetAttribute("split-min", min);
	elem->SetAttribute("split-max", max);
	horizontal->InsertEndChild(elem);
	return elem;
}

static XMLElement* newHorizontalChild(XMLDocument& doc, const std::string& id)
{
	XMLElement* elem = doc.NewElement("horizontal");
	elem->SetAttribute("id", id.c_str());
	return elem;
}

TEST_F(HorizontalLayoutTest, EmptyTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	LayoutFactory factory(log_trace, weak_ptr<World>(), fname, &doc, false);
	shared_ptr<Layout> root = factory.parseTree("test");
	ASSERT_EQ(root->getChildCount(), 0);
	ASSERT_TRUE(util::startsWith(root->toString(), "(HorizontalLayout"));
}

TEST_F(HorizontalLayoutTest, BasicLayoutTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	addHorizontalChild(doc, hor, "empty", 2);
	addHorizontalChild(doc, hor, "empty", 3);
	addHorizontalChild(doc, hor, "empty", 1);

	LayoutFactory factory(log_trace, weak_ptr<World>(), fname, &doc, false);
	shared_ptr<Layout> root = factory.parseTree("test");

	root->measure(geom::Box(100, 100));
	root->layout(geom::Box(100, 100));

	ASSERT_EQ(3, root->getChildCount());

	ASSERT_NEAR(200.0/6, root->getChildAt(0).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(0).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(300.0/6, root->getChildAt(1).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(1).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(100.0/6, root->getChildAt(2).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(2).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(100, root->size().width(), 1.0/10);
	ASSERT_NEAR(100, root->size().height(), 1.0/10);
}

TEST_F(HorizontalLayoutTest, MixedTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	addHorizontalChild(doc, hor, "empty", 2);
	addHorizontalChild(doc, hor, "empty", geom::Unspecified, 10, 50);
	addHorizontalChild(doc, hor, "empty", 1);

	LayoutFactory factory(log_trace, weak_ptr<World>(), fname, &doc, false);
	shared_ptr<Layout> root = factory.parseTree("test");

	root->measure(geom::Box(100, 100));
	root->layout(geom::Box(100, 100));

	ASSERT_EQ(3, root->getChildCount());

	ASSERT_NEAR(100.0/3, root->getChildAt(0).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(0).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(50.0, root->getChildAt(1).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(1).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(50.0/3, root->getChildAt(2).lock()->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(2).lock()->size().height(), 1.0/10);

	ASSERT_NEAR(100, root->size().width(), 1.0/10);
	ASSERT_NEAR(100, root->size().height(), 1.0/10);
}


}}
