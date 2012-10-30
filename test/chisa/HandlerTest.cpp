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

class TestFix {
	HANDLER_KLASS_WEAK(TestFix);
private:
	int expired_;
public:
	TestFix():HANDLER_KLASS_WEAK_INIT{
		this->expired_ = 0;
	}
	int expired() const noexcept {
		return this->expired_;
	}
};

void TestFix::onFree() {
	if(this->weakEntity_){
		this->weakEntity_->notifyDead();
	}
	this->expired_++;
}

TEST(WeakHandlerTest, HandlerTest)
{
	TestFix* test = new TestFix();
	{
		Handler<TestFix> handler(test);
	}
	ASSERT_EQ(1, test->expired());
	delete test;
}

TEST(WeakHandlerTest, WeakHandlerTest)
{
	TestFix* test = new TestFix();
	{
		Handler<TestFix> handler(test);
		{
			HandlerW<TestFix> weak(handler);
			handler.reset();
			ASSERT_FALSE(handler);
			ASSERT_TRUE(weak.expired());
		}
	}
	ASSERT_EQ(1, test->expired());
	delete test;
}

}

