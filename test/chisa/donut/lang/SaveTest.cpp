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

TEST(SaveTest, ClockSaveTest)
{
	INIT_DONUT;
	Handler<Clock> clock(new Clock(donut.get()));
	std::string src;
	clock->tick();
	clock->tick();
	clock->tick();
	clock->tick();
	clock->seek(clock->lastTime()-2);
	clock->seek(clock->lastTime()-4);
	{
		Handler<util::XObject> obj = clock->save().get<util::XObject>();
		tinyxml2::XMLDocument doc;
		doc.InsertFirstChild(obj->toXML(&doc));
		tinyxml2::XMLPrinter printer;
		doc.Print(&printer);
		src = printer.CStr();
	}
	Handler<Clock> clock2(new Clock(donut.get()));
	{
		tinyxml2::XMLDocument doc;
		doc.Parse(src.c_str());
		clock2->load(Handler<util::XObject>(new util::XObject(doc.RootElement())));
	}
	ASSERT_EQ(clock->now(), clock2->now());
	ASSERT_EQ(clock->lastTime(), clock2->lastTime());
	ASSERT_EQ(clock->firstTime(), clock2->firstTime());
}

TEST(SaveTest, DonutSaveTest)
{
	INIT_DONUT;
	tinyxml2::XMLDocument doc;
	Handler<util::XObject> obj = donut->save().get<util::XObject>();
	doc.InsertEndChild(obj->toXML(&doc));
	tinyxml2::XMLPrinter printer;
	doc.Print(&printer);
	std::string src = printer.CStr();
//	std::cout << src << std::endl;
}

}}


