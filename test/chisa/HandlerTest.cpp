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
	int* deleted_;
	std::function<void(void)> onDelete;
public:
	TestFix( int* expired, int* deleted)
	:expired_(expired)
	,deleted_(deleted)
	,onDelete([](){})
	{
		*this->expired_ = 0;
		*this->deleted_ = 0;
	}
	virtual ~TestFix() noexcept {
		(*this->deleted_)++;
		this->onDelete();
	}
	void onFree() {
		(*this->expired_)++;
		delete this;
	}
	void setDestroyCallback(std::function<void(void)> f){
		this->onDelete = f;
	}
};

TEST(HandlerTest, HandlerTest)
{
	int e = 0;
	int d = 0;
	TestFix* test = new TestFix(&e, &d);
	{
		Handler<TestFix> handler(test);
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, WeakHandlerTest)
{
	int e = 0;
	int d = 0;
	TestFix* test = new TestFix(&e, &d);
	{
		Handler<TestFix> handler(test);
		{
			HandlerW<TestFix> weak(handler);
			handler.reset();
			ASSERT_FALSE(handler);
			ASSERT_TRUE(weak.expired());
		}
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, TouchWeakHandlerOnDestroyTest)
{
	int e = 0;
	int d = 0;
	HandlerW<TestFix> weak;
	{
		Handler<TestFix> handler( new TestFix(&e, &d) );
		weak = handler;
		ASSERT_FALSE(weak.expired());
		handler->setDestroyCallback([&weak](){
			SCOPED_TRACE("in lambda");
			ASSERT_TRUE(weak.expired());
		});
	}
	ASSERT_TRUE(weak.expired());
}

}
