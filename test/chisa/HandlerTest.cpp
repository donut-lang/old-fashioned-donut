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
		ASSERT_EQ(1, handler.refcount());
		ASSERT_TRUE(static_cast<bool>(handler));
		ASSERT_TRUE(!!handler);
		ASSERT_EQ(test, handler.get());
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, NullHandlerTest)
{
	Handler<TestFix> f;
	ASSERT_FALSE(f);
	ASSERT_EQ(nullptr, f.get());
	ASSERT_EQ(0, f.refcount());
}

TEST(HandlerTest, HandlerDuplicationTest)
{
	int e = 0;
	int d = 0;
	TestFix* test = new TestFix(&e, &d);
	{
		Handler<TestFix> handler(test);
		ASSERT_EQ(1, handler.refcount());
		ASSERT_ANY_THROW(Handler<TestFix> handler2(test));
		ASSERT_EQ(1, handler.refcount());
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, HandlerCopyConstructorTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> handler( new TestFix(&e, &d) );
		ASSERT_EQ(1, handler.refcount());
		{
			Handler<TestFix> handler2( handler );
			ASSERT_EQ(2, handler.refcount());
			ASSERT_EQ(2, handler2.refcount());
		}
		ASSERT_EQ(1, handler.refcount());
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, AssignOperatorTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> handler( new TestFix(&e, &d) );
		ASSERT_EQ(1, handler.refcount());
		{
			Handler<TestFix> handler2 = handler;
			ASSERT_EQ(2, handler.refcount());
			ASSERT_EQ(2, handler2.refcount());
		}
		{
			Handler<TestFix> handler2;
			handler2 = handler;
			ASSERT_EQ(2, handler.refcount());
			ASSERT_EQ(2, handler2.refcount());
		}
		ASSERT_EQ(1, handler.refcount());
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, RValueTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> handler( new TestFix(&e, &d) );
		ASSERT_EQ(1, handler.refcount());
		{
			Handler<TestFix> handler2 ( ([](Handler<TestFix> f)->Handler<TestFix>{ return f; })(handler) );
			ASSERT_EQ(2, handler.refcount());
			ASSERT_EQ(2, handler2.refcount());
		}
		{
			Handler<TestFix> handler2;
			handler2 = Handler<TestFix>(handler);
			ASSERT_EQ(2, handler.refcount());
			ASSERT_EQ(2, handler2.refcount());
		}
		ASSERT_EQ(1, handler.refcount());
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, SwapTest)
{
	int e1 = 0;
	int d1 = 0;
	int e2 = 0;
	int d2 = 0;
	{
		auto t1 = new TestFix(&e1, &d1);
		auto t2 = new TestFix(&e2, &d2);
		Handler<TestFix> handler1( t1 );
		Handler<TestFix> handler11( handler1 );
		Handler<TestFix> handler2( t2 );
		ASSERT_EQ(2, handler1.refcount());
		ASSERT_EQ(t1, handler1.get());
		ASSERT_EQ(2, handler11.refcount());
		ASSERT_EQ(t1, handler11.get());
		ASSERT_EQ(1, handler2.refcount());
		ASSERT_EQ(t2, handler2.get());

		handler1.swap(handler2);
		ASSERT_EQ(1, handler1.refcount());
		ASSERT_EQ(t2, handler1.get());
		ASSERT_EQ(2, handler11.refcount());
		ASSERT_EQ(t1, handler11.get());
		ASSERT_EQ(2, handler2.refcount());
		ASSERT_EQ(t1, handler2.get());

	}
	ASSERT_EQ(1, e1);
	ASSERT_EQ(1, d1);
	ASSERT_EQ(1, e2);
	ASSERT_EQ(1, d2);

}

TEST(HandlerTest, HandlerResetTest)
{
	int e = 0;
	int d = 0;
	{
		ASSERT_EQ(0, e);
		ASSERT_EQ(0, d);
		Handler<TestFix> handler(new TestFix(&e, &d));
		ASSERT_EQ(1, handler.refcount());
		ASSERT_TRUE(static_cast<bool>(handler));
		handler.reset();
		ASSERT_EQ(0, handler.refcount());
		ASSERT_FALSE(handler);
		ASSERT_EQ(1, e);
		ASSERT_EQ(1, d);
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, WeakHandlerTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> handler(new TestFix(&e, &d));
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
			ASSERT_FALSE(weak.lock());
			ASSERT_TRUE(weak.expired());
		});
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
	ASSERT_FALSE(weak.lock());
	ASSERT_TRUE(weak.expired());
}

}
