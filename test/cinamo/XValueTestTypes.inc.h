/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

{ /* プラス */\
	TYPE const uvar = VALUE;\
	XValue x (uvar);\
	ASSERT_NO_THROW(x.as<TYPE>());\
	ASSERT_ANY_THROW(x.as<NOT_TYPE>());\
	TYPE var;\
	NOT_TYPE nvar;\
	ASSERT_ANY_THROW(x.as(nvar));\
	ASSERT_EQ(VALUE, x.as(var));\
	ASSERT_EQ(VALUE, var);\
}\
{ /* マイナス */\
	TYPE const uvar = -(VALUE);\
	XValue x (uvar);\
	ASSERT_NO_THROW(x.as<TYPE>());\
	ASSERT_ANY_THROW(x.as<NOT_TYPE>());\
	TYPE var;\
	NOT_TYPE nvar;\
	ASSERT_ANY_THROW(x.as(nvar));\
	ASSERT_EQ(static_cast<TYPE>(-(VALUE)), x.as(var));\
	ASSERT_EQ(static_cast<TYPE>(-(VALUE)), var);\
}

