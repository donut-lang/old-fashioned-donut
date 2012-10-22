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
#include "../../src/chisa/tk/widget/Content/Node.h"
#include "../../src/chisa/tk/widget/Content/NodeReader.h"
#include <tinyxml2.h>
#include <typeinfo>

namespace chisa {
namespace tk {
namespace widget {

std::shared_ptr<tinyxml2::XMLDocument> parse(const std::string& str)
{
	std::shared_ptr<tinyxml2::XMLDocument> d(new tinyxml2::XMLDocument);
	d->Parse(str.c_str(), str.size());
	return d;
}

TEST(NodeReadingTest, NOPTest)
{
	auto doc = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	)delimiter");
	NodeReader reader;
	EXPECT_ANY_THROW(reader.parseTree(doc->RootElement()));
}

TEST(NodeReadingTest, TextNodeTest)
{
	auto doc = parse(R"delimiter(
<?xml version="1.0" encoding="UTF-8"?>
<doc>Kitty on your lap.</doc>
)delimiter");
	NodeReader reader;
	std::shared_ptr<Document> docNode;
	ASSERT_NO_THROW(docNode = reader.parseTree(doc->RootElement()));
	ASSERT_TRUE(docNode.get());
	ASSERT_EQ(docNode, docNode->root().lock());
	ASSERT_EQ(nullptr, docNode->parent().lock().get());

	std::shared_ptr<Node> child(docNode->at(0));
	ASSERT_EQ(typeid(Text), typeid(*(child.get())));
	ASSERT_EQ(docNode, child->root().lock());
	ASSERT_EQ(docNode, child->parent().lock());
	std::shared_ptr<Text> t = std::dynamic_pointer_cast<Text>(child);

	ASSERT_TRUE(t.get());
	ASSERT_EQ("Kitty on your lap.", t->text());
}

TEST(ContentWidget, HeadingTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><h1>a</h1><h2>b</h2><h3>c</h3></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_TRUE(m.get());
	ASSERT_EQ(3, m->count());
	int cnt = 0;
	for(TreeNode::Iterator it = m->begin(); it != m->end(); ++it){
		std::shared_ptr<Node> node(*it);
		std::shared_ptr<Heading> hnode(std::dynamic_pointer_cast<Heading>(node));
		ASSERT_TRUE(hnode.get());
		ASSERT_EQ(++cnt, hnode->level());
	}
}

}}}

