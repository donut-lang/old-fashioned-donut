/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/MixType.h>

#include <cfloat>
#include <cmath>

namespace cinamo {

class A{
public:
	const int a;
	A(int x):a(x){

	}
};
class B{
public:
	const int b;
	B(int x):b(x){

	}
};

class AB : public A, public B{
public:
	AB(int a,int b):A(a),B(b){

	}
};

TEST(MixTypeTest, Basic)
{
	AND<A,B> ab (new AB(1,2));
	ASSERT_EQ(1, ab.asLeft()->a);
	ASSERT_EQ(2, ab.asRight()->b);
}

}
