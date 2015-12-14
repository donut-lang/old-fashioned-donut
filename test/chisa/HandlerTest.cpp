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

#include "../TestCommon.hpp"
#include <cinamo/Handler.h>

namespace cinamo {

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
	bool onFree() {
		(*this->expired_)++;
		return false;
	}
	void setDestroyCallback(std::function<void(void)> f){
		this->onDelete = f;
	}
};

class TestFixDerived : public TestFix {
public:
	TestFixDerived( int* expired, int* deleted):TestFix(expired, deleted)
	{
	}
};

TEST(HandlerTest, SizeConstraintTest)
{
	static_assert(sizeof(TestFix*) == sizeof(Handler<TestFix>), "size of Handler must be equal to the pointer.");
	static_assert(sizeof(HandlerBody<TestFix>) <= sizeof(void*)*4, "Overhead of HandlerBody must be less then or equal to 4 pointers.");
}

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
			ASSERT_EQ(1, handler.refcount());
			HandlerW<TestFix> weak(handler);
			ASSERT_FALSE(weak.expired());
			ASSERT_EQ(1, weak.refcount());
			ASSERT_EQ(handler, weak.lock());
			ASSERT_EQ(1, handler.refcount());
			ASSERT_EQ(1, weak.refcount());
			handler.reset();
			ASSERT_EQ(0, handler.refcount());
			ASSERT_EQ(1, weak.refcount());
			ASSERT_FALSE(handler);
			ASSERT_TRUE(weak.expired());
		}
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, WeakHandlerUpCastTest)
{
	int e = 0;
	int d = 0;
	HandlerW<TestFix> weak;
	{
		Handler<TestFixDerived> handler( new TestFixDerived(&e, &d) );
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

TEST(HandlerTest, UpCastConstructorTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> dhandler( new TestFixDerived(&e, &d) );
		ASSERT_TRUE(static_cast<bool>(dhandler));
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, UpCastTest)
{
	int e = 0;
	int d = 0;
	{
		Handler<TestFix> bhandler( new TestFix(&e, &d) );
		Handler<TestFixDerived> dhandler( new TestFixDerived(&e, &d) );
		bhandler = dhandler;
		ASSERT_TRUE(static_cast<bool>(bhandler));
	}
	ASSERT_EQ(2, e);
	ASSERT_EQ(2, d);
}

TEST(HandlerTest, VectorTest)
{
	int e = 0;
	int d = 0;
	{
		std::vector<Handler<TestFix> > vec;
		Handler<TestFix> h( new TestFix(&e, &d) );
		vec.push_back(h);
		vec.push_back(h);
		vec.push_back(h);
		vec[2] = h;
		vec.at(2) = h;
		h = vec[2];
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

TEST(HandlerTest, ReferenceTest)
{
	int e = 0;
	int d = 0;
	{
		std::vector<Handler<TestFix> > vec;
		Handler<TestFix> h( new TestFix(&e, &d) );
		Handler<TestFix>& hr = h;
		vec.push_back(hr);
		vec.push_back(hr);
		vec.push_back(hr);
		vec[2] = hr;
		vec.at(2) = hr;
		hr = vec[2];
	}
	ASSERT_EQ(1, e);
	ASSERT_EQ(1, d);
}

}
