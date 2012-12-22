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

#include "DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {

TEST(SaveTest, DonutSaveTest)
{
	std::string src;
	{
		INIT_DONUT;
		machine->start(donut->parse("Global.val = {}; Global.val.x=1+1;"));
		tinyxml2::XMLDocument doc;
		Handler<util::XObject> obj = donut->save().as<util::XObject>();
		doc.InsertEndChild(obj->toXML(&doc));
		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		src = printer.CStr();
	}
	//std::cout << src << std::endl;
	{
		Handler<Donut> donut(new Donut(log_trace));
		tinyxml2::XMLDocument doc;
		doc.Parse(src.c_str());
		util::XValue v = util::XValue::fromXML(doc.RootElement());
		donut->load(v);
		Handler<Object> obj = donut->queryMachine()->start(donut->parse("Global.val.x;"));
		ASSERT_TRUE(obj->isInt());
		ASSERT_EQ(2, obj->toInt(donut->heap()));
	}
}

TEST(SaveTest, ClosureRestoreTest)
{
	std::string src;
	{
		INIT_DONUT;
		machine->start(donut->parse("Global.val = {}; Global.val.x=func(x,y){x+y;};"));
		tinyxml2::XMLDocument doc;
		Handler<util::XObject> obj = donut->save().as<util::XObject>();
		doc.InsertEndChild(obj->toXML(&doc));
		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		src = printer.CStr();
	}
	//std::cout << src << std::endl;
	{
		Handler<Donut> donut(new Donut(log_trace));
		tinyxml2::XMLDocument doc;
		doc.Parse(src.c_str());
		util::XValue v = util::XValue::fromXML(doc.RootElement());
		donut->load(v);
		Handler<Object> obj = donut->queryMachine()->start(donut->parse("Global.val.x(1,2);"));
		ASSERT_TRUE(obj->isInt());
		ASSERT_EQ(3, obj->toInt(donut->heap()));
	}
}

TEST(SaveTest, ClosureSeekAndSaveLoadTest)
{
	std::string src;
	timestamp_t tm1;
	timestamp_t tm2;
	{
		INIT_DONUT;
		machine->start(donut->parse("Global.val = {}; Global.val.x=func(x,y){x+y;};"));
		tm1 = donut->nowTime();
		machine->start(donut->parse("Global.val = {}; Global.val.x=10;"));
		tm2 = donut->nowTime();
		donut->seek(tm1);
		tinyxml2::XMLDocument doc;
		Handler<util::XObject> obj = donut->save().as<util::XObject>();
		doc.InsertEndChild(obj->toXML(&doc));
		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		src = printer.CStr();
	}
	//std::cout << src << std::endl;
	{
		Handler<Donut> donut(new Donut(log_trace));
		tinyxml2::XMLDocument doc;
		doc.Parse(src.c_str());
		util::XValue v = util::XValue::fromXML(doc.RootElement());
		donut->load(v);
		ASSERT_EQ(tm1, donut->nowTime());
		{
			Handler<Object> obj = donut->heap()->getGlobalObject("val")->get(donut->heap(),"x");
			ASSERT_TRUE(obj->isObject());
		}
		donut->seek(tm2);
		{
			Handler<Object> obj = donut->queryMachine()->start(donut->parse("Global.val.x;"));
			ASSERT_TRUE(obj->isInt());
			ASSERT_EQ(10, obj->toInt(donut->heap()));
		}
		donut->seek(tm1);
		{
			Handler<Object> obj = donut->queryMachine()->start(donut->parse("Global.val.x(1,2);"));
			ASSERT_TRUE(obj->isInt());
			ASSERT_EQ(3, obj->toInt(donut->heap()));
		}
	}
}

TEST(SaveTest, SaveOnInterruptTest)
{
	std::string src;
	{
		INIT_DONUT;
		machine->start(donut->parse("Global.x = {}; x.val = 10; x.val=interrupt \"str\"; x.val;"));
		ASSERT_TRUE(donut->queryMachine()->isInterrupted());
		tinyxml2::XMLDocument doc;
		util::XValue v = donut->save().as<util::XObject>();
		doc.InsertEndChild(v.toXML(&doc));
		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		src = printer.CStr();
	}
	//std::cout << src << std::endl;
	{
		Handler<Donut> donut(new Donut(log_trace));
		tinyxml2::XMLDocument doc;
		doc.Parse(src.c_str());
		util::XValue v = util::XValue::fromXML(doc.RootElement());
		donut->load(v);
		ASSERT_TRUE(donut->queryMachine()->isInterrupted());
		{
			Handler<Object> obj = donut->heap()->getGlobalObject("x")->get(donut->heap(),"val");
			ASSERT_TRUE(obj->isInt());
			ASSERT_EQ(10, obj->toInt(donut->heap()));
		}
		{
			Handler<Object> obj = donut->queryMachine()->resume(donut->heap()->createFloatObject(10.123));
			ASSERT_TRUE(obj->isObject());
			ASSERT_FLOAT_EQ(10.123, obj->toFloat(donut->heap()));
		}
		{
			Handler<Object> obj = donut->heap()->getGlobalObject("x")->get(donut->heap(),"val");
			ASSERT_TRUE(obj->isObject());
			ASSERT_FLOAT_EQ(10.123, obj->toFloat(donut->heap()));
		}
	}
}
}}


