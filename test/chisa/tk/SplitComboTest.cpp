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


#include "../../TestCommon.hpp"
#include "../../chisa/tk/element/SplitCombo.hpp"
#include "../../chisa/tk/element/ElementFactory.hpp"

namespace chisa {
namespace tk {

using namespace std;
using namespace tinyxml2;
const string fname = "memory";
class HorizontalComboTest : public ::testing::Test
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

static XMLElement* addHorizontalChild(XMLDocument& doc, XMLElement* horizontal, string const& elemname, float weight=geom::Unspecified, float min=geom::Unspecified, float max=geom::Unspecified)
{
	XMLElement* elem = doc.NewElement(elemname.c_str());
	elem->SetAttribute("split-weight", weight);
	if(geom::isSpecified(min)){
		elem->SetAttribute("split-min", min);
	}
	if(geom::isSpecified(max)){
		elem->SetAttribute("split-max", max);
	}
	horizontal->InsertEndChild(elem);
	return elem;
}

static XMLElement* newHorizontalChild(XMLDocument& doc, std::string const& id)
{
	XMLElement* elem = doc.NewElement("horizontal");
	elem->SetAttribute("id", id.c_str());
	return elem;
}

TEST_F(HorizontalComboTest, EmptyTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	ElementFactory factory(log_trace, chisa::HandlerW<World>(), fname, &doc, false);
	Handler<ElementGroup> root = factory.parseTree("test").cast<ElementGroup>();
	ASSERT_EQ(root->getChildCount(), 0);
	ASSERT_TRUE(startsWith(root->toString(), "(HorizontalCombo"));
}

TEST_F(HorizontalComboTest, BasicLayoutTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	addHorizontalChild(doc, hor, "empty", 2);
	addHorizontalChild(doc, hor, "empty", 3);
	addHorizontalChild(doc, hor, "empty", 1);

	ElementFactory factory(log_trace, chisa::HandlerW<World>(), fname, &doc, false);
	Handler<ElementGroup> root = factory.parseTree("test").cast<ElementGroup>();

	root->measure(geom::Box(100, 100));
	root->layout(geom::ZERO, geom::Box(100, 100));

	ASSERT_EQ(3, root->getChildCount());

	ASSERT_NEAR(200.0/6, root->getChildAt(0)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(0)->size().height(), 1.0/10);

	ASSERT_NEAR(300.0/6, root->getChildAt(1)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(1)->size().height(), 1.0/10);

	ASSERT_NEAR(100.0/6, root->getChildAt(2)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(2)->size().height(), 1.0/10);

	ASSERT_NEAR(100, root->size().width(), 1.0/10);
	ASSERT_NEAR(100, root->size().height(), 1.0/10);
}

TEST_F(HorizontalComboTest, MixedTest)
{
	XMLElement* hor = newHorizontalChild(doc, "test");
	world->InsertFirstChild(hor);
	addHorizontalChild(doc, hor, "empty", 2);
	addHorizontalChild(doc, hor, "empty", geom::Unspecified, 10, 50);
	addHorizontalChild(doc, hor, "empty", 1);

	ElementFactory factory(log_trace, chisa::HandlerW<World>(), fname, &doc, false);
	Handler<ElementGroup> root = factory.parseTree("test").cast<ElementGroup>();

	root->measure(geom::Box(100, 100));
	root->layout(geom::ZERO, geom::Box(100, 100));

	ASSERT_EQ(3, root->getChildCount());

	ASSERT_NEAR(50.0, root->getChildAt(0)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(0)->size().height(), 1.0/10);

	ASSERT_NEAR(25.0, root->getChildAt(1)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(1)->size().height(), 1.0/10);

	ASSERT_NEAR(25.0, root->getChildAt(2)->size().width(), 1.0/10);
	ASSERT_NEAR(100.0, root->getChildAt(2)->size().height(), 1.0/10);

	ASSERT_NEAR(100, root->size().width(), 1.0/10);
	ASSERT_NEAR(100, root->size().height(), 1.0/10);
}


}}
