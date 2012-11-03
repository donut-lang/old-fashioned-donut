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
#include "../../src/chisa/doc/node/Node.h"
#include "../../src/chisa/doc/node/NodeReader.h"
#include <typeinfo>

namespace chisa {
namespace doc {

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
	ASSERT_EQ(docNode.get(), docNode->root());
	ASSERT_EQ(nullptr, docNode->parent());

	Node* child(docNode->at(0));
	ASSERT_EQ(typeid(Text), typeid(*(child)));
	ASSERT_EQ(docNode.get(), child->root());
	ASSERT_EQ(docNode.get(), child->parent());
	Text* t = dynamic_cast<Text*>(child);

	ASSERT_TRUE(t);
	ASSERT_EQ("Kitty on your lap.", t->text());
}

TEST(NodeReadingTest, HeadingTest)
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
		Node* node(*it);
		Heading* hnode(dynamic_cast<Heading*>(node));
		ASSERT_TRUE(hnode);
		ASSERT_EQ(++cnt, hnode->level());
	}
}

TEST(NodeReadingTest, MixedTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><h1>a</h1>kitty on your lap<h3>c</h3></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_TRUE(m.get());
	ASSERT_EQ(3, m->count());

	Heading* hnode(dynamic_cast<Heading*>(m->at(0)));
	ASSERT_TRUE(hnode);
	ASSERT_EQ(1, hnode->level());
	ASSERT_EQ("a", dynamic_cast<Text*>(hnode->at(0))->text());

	hnode = dynamic_cast<Heading*>(m->at(2));
	ASSERT_TRUE(hnode);
	ASSERT_EQ(3, hnode->level());
	ASSERT_EQ("c", dynamic_cast<Text*>(hnode->at(0))->text());

	Text* tnode(dynamic_cast<Text*>(m->at(1)));
	ASSERT_TRUE(tnode);
	ASSERT_EQ("kitty on your lap", tnode->text());
}

TEST(NodeReadingTest, AttributeTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><p id="test"></p></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_EQ(1, m->count());

	Paragraph* p = dynamic_cast<Paragraph*>(m->at(0));
	ASSERT_TRUE(p);
	ASSERT_EQ("test", p->id());
	ASSERT_EQ(0, p->count());
}

TEST(NodeReadingTest, SpaceStringTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc>               <p 	>	</p>	</doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_EQ(1, m->count());

	Paragraph* p(dynamic_cast<Paragraph*>(m->at(0)));
	ASSERT_TRUE(p);
	ASSERT_EQ(0, p->count());
}

TEST(NodeReadingTest, TreeNodeNotFoundTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><p></p><br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_FALSE(m->findTreeNodeById("test"));
	ASSERT_FALSE(m->findTreeNodeById(""));

	tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><p id="test"></p><br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_FALSE(m->findTreeNodeById("test1"));
	ASSERT_FALSE(m->findTreeNodeById("tes"));
	ASSERT_FALSE(m->findTreeNodeById(""));
}

TEST(NodeReadingTest, TreeNodeFoundTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc id="root"><p></p><br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_TRUE(m->findTreeNodeById("root"));
	ASSERT_TRUE(dynamic_cast<Document*>(m->findTreeNodeById("root")));

	tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><p id="test"></p><br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_TRUE(m->findTreeNodeById("test"));
	ASSERT_TRUE(dynamic_cast<Paragraph*>(m->findTreeNodeById("test")));
}

TEST(NodeReadingTest, NoTextFoundTest)
{
	std::shared_ptr<Document> m;
	auto tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc><p></p><br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_FALSE(m->findFirstTextNode());

	tree = parse(R"delimiter(
	<?xml version="1.0" encoding="UTF-8"?>
	<doc>a<p id="test">b</p>c<br/><br/></doc>
	)delimiter");
	ASSERT_NO_THROW(m = NodeReader().parseTree(tree->RootElement()));
	ASSERT_TRUE(m->findFirstTextNode());
	ASSERT_EQ("a", m->findFirstTextNode()->text());

	ASSERT_TRUE(m->findTreeNodeById("test")->findFirstTextNode());
	ASSERT_EQ("b", m->findTreeNodeById("test")->findFirstTextNode()->text());
}

}}
