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

#include "lang/DonutHelper.h"
#include <math.h>

namespace chisa {
namespace donut {

TEST(ClockTest, InvalidTimeSeekTest)
{
	INIT_DONUT;
	Handler<Clock> clock(new Clock(donut.get()));
	auto t = clock->now();
	std::string src;
	clock->tick();
	clock->tick();
	clock->tick();
	clock->tick();
	auto t2 = clock->now();

	ASSERT_ANY_THROW( clock->seek(t-1) );
	ASSERT_ANY_THROW( clock->seek(t2+1) );
}

TEST(ClockTest, DiscardTest)
{
	INIT_DONUT;
	Handler<Clock> clock(new Clock(donut.get()));
	clock->tick();
	clock->tick();
	auto t = clock->now();
	clock->tick();
	clock->tick();
	clock->seek(t);
	clock->discardFuture();
	ASSERT_EQ(t, clock->lastTime());
	ASSERT_NO_THROW( clock->discardFuture() );
	clock->discardHistory();
	ASSERT_EQ(t, clock->firstTime());
	ASSERT_NO_THROW( clock->discardHistory() );
}

TEST(ClockTest, WithDeadDonutTest)
{
	Handler<Clock> clock;
	unsigned int t;
	{
		INIT_DONUT;
		clock = Handler<Clock>(new Clock(donut.get()));
		clock->tick();
		clock->tick();
		t = clock->now();
		clock->tick();
		clock->tick();
		clock->seek(t);
	}
	ASSERT_NO_THROW( clock->seek(t) );
	ASSERT_NO_THROW( clock->tick() );
	ASSERT_NO_THROW( clock->discardFuture() );
	ASSERT_NO_THROW( clock->discardHistory() );
}

TEST(ClockTest, ClockSaveTest)
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
		Handler<util::XObject> obj = clock->save().as<util::XObject>();
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

}}


