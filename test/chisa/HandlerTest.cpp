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

#include "../TestCommon.h"
#include "../../src/chisa/Handler.h"

namespace chisa {

class TestFix : public HandlerBody<TestFix> {
private:
	int* expired_;
public:
	TestFix( int* expired ):expired_(expired) {
		*this->expired_ = 0;
	}
	void onFree() {
		(*this->expired_)++;
		delete this;
	}
};

TEST(WeakHandlerTest, HandlerTest)
{
	int c = 0;
	TestFix* test = new TestFix(&c);
	{
		Handler<TestFix> handler(test);
	}
	ASSERT_EQ(1, c);
}

TEST(WeakHandlerTest, WeakHandlerTest)
{
	int c = 0;
	TestFix* test = new TestFix(&c);
	{
		Handler<TestFix> handler(test);
		{
			HandlerW<TestFix> weak(handler);
			handler.reset();
			ASSERT_FALSE(handler);
			ASSERT_TRUE(weak.expired());
		}
	}
	ASSERT_EQ(1, c);
}

}

