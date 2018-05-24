/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/functional/Base.h>

namespace cinamo {

TEST(TypeTransTest, Lambda2Functor){
	auto z = [](){};
	static_assert(
			std::is_same<
				decltype(makeFunctor( z )),
				std::function<void()>
				>::value, "cannot deduce std::function");

	auto d = [](int x){ return x*x; };
	static_assert(
			std::is_same<
				decltype(makeFunctor( d )),
				std::function<int(int)>
				>::value, "cannot deduce std::function");

	std::function<int(int)> f = makeFunctor( [](int x){ return x*x; } );
	ASSERT_EQ( 4, f(2) );
}

TEST(TypeTransTest, Functor2Functor){
	std::function<bool(bool)> _not ( [](bool a){ return !a; } );
	static_assert(
			std::is_same<
				decltype(makeFunctor( _not )),
				std::function<bool(bool)> const&
				>::value, "cannot deduce std::function");

	ASSERT_FALSE( _not(true) );
	ASSERT_TRUE( makeFunctor(_not)(false) );
}

TEST(TypeTransTest, Object2Functor){
	class Test {
	public:
		bool operator()(int a, int b, int c) const{
			return a==b && b == c && c == a;
		}
	};
	static_assert(
			std::is_same<
				decltype(makeFunctor(Test())),
				std::function<bool(int,int,int)> >::value, "deduce failed.");
	auto f = makeFunctor(Test());
	ASSERT_FALSE( f(1,2,3) );
	ASSERT_TRUE( f(1,1,1) );
}

TEST(TypeTransTest, Method2Functor){
	class Test {
	public:
		bool call(float x, float y){
			return x*y > 0;
		}
		bool constCall(float x, float y) const{
			return x*y > 0;
		}
	};
	static_assert(
			std::is_same<
				decltype(makeFunctor(&Test::call)),
				std::function<bool(Test*, float, float)> >::value, "deduce failed.");
	static_assert(
			std::is_same<
				decltype(makeFunctor(&Test::constCall)),
				std::function<bool(Test const*, float, float)> >::value, "deduce failed.");
}

namespace {
int func( int a, int b ){
	return a*b;
}

TEST(TypeTransTest, Function2Functor){
	static_assert(
			std::is_same<
				decltype(makeFunctor(func)),
				std::function<int(int, int)> >::value, "deduce failed.");

	auto f = makeFunctor(func);
	ASSERT_EQ(12, f(3,4));
}
}

}
