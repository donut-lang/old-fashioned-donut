/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Dynamic.h>

namespace cinamo {

TEST(DemangleTest, Literal)
{
	int d=2;
	ASSERT_EQ("int", demangle(d));
	ASSERT_EQ("int", demangle(&d));
	ASSERT_EQ("int", demangle<int>());
	ASSERT_EQ("int", demangle<int&>());
	ASSERT_EQ("int", demangle<int&&>());
	ASSERT_EQ("int", demangle<int*>());
	ASSERT_EQ("int", demangle<int**>());
	ASSERT_EQ("int", demangle<int***>());

	float f=0.0f;
	ASSERT_EQ("float", demangle(f));
}

class __DemangleTest__SampleVirtual {
public:
	__DemangleTest__SampleVirtual() = default;
	virtual ~__DemangleTest__SampleVirtual() noexcept = default;
};

class __DemangleTest__Sample {
public:
	__DemangleTest__Sample() = default;
	~__DemangleTest__Sample() noexcept = default;
};
class __DemangleTest__SampleVirtualDerived : public __DemangleTest__SampleVirtual{
	int x;
public:
	__DemangleTest__SampleVirtualDerived() = default;
	virtual ~__DemangleTest__SampleVirtualDerived() noexcept = default;
};

class __DemangleTest__SampleDerived : public __DemangleTest__Sample{
	int x;
public:
	__DemangleTest__SampleDerived() = default;
	~__DemangleTest__SampleDerived() noexcept = default;
};

TEST(DemangleTest, Class)
{
	__DemangleTest__SampleVirtual s;
	__DemangleTest__SampleVirtual* sp = &s;
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtual", demangle(s));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtual", demangle(&s));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtual", demangle(sp));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtual", demangle(&sp));

	__DemangleTest__Sample ss;
	__DemangleTest__Sample* ssp = &ss;
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(ss));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(&ss));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(ssp));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(&ssp));
}

template <typename T>
class __DemangleTest__TemplateKlass {
};

TEST(DemangleTest, TemplateClass)
{
	__DemangleTest__TemplateKlass<__DemangleTest__SampleVirtual> s;
	ASSERT_EQ("cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__SampleVirtual>", demangle(s));

	__DemangleTest__TemplateKlass<__DemangleTest__Sample> ss;
	ASSERT_EQ("cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__Sample>", demangle(ss));
}

TEST(DemangleTest, RecusriveTemplateClass)
{
	cinamo::__DemangleTest__TemplateKlass<__DemangleTest__TemplateKlass<__DemangleTest__SampleVirtual>> s;
	ASSERT_EQ("cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__SampleVirtual> >", demangle(s));

	cinamo::__DemangleTest__TemplateKlass<__DemangleTest__TemplateKlass<__DemangleTest__Sample>> ss;
	ASSERT_EQ("cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__TemplateKlass<cinamo::__DemangleTest__Sample> >", demangle(ss));
}

TEST(DemangleTest, DerivedVirtualKlassTest)
{
	__DemangleTest__SampleVirtualDerived s;
	__DemangleTest__SampleVirtual* p = &s;
	ASSERT_EQ(demangle(typeid(s)), demangle(typeid(*p)));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtualDerived", demangle(typeid(*p)));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtualDerived", demangle(p));
	ASSERT_EQ("cinamo::__DemangleTest__SampleVirtualDerived", demangle(&p));
}

TEST(DemangleTest, DerivedKlassTest)
{
	// ポリモルフィックでないクラスは、ポインタを戻してもベースポインタにしかならない。
	__DemangleTest__SampleDerived s;
	__DemangleTest__Sample* p = &s;
	ASSERT_NE(demangle(typeid(s)), demangle(typeid(*p)));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(typeid(*p)));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(p));
	ASSERT_EQ("cinamo::__DemangleTest__Sample", demangle(&p));
}

namespace {
class AnomTest {
};

TEST(DemangleTest, AnomNameSpace)
{
	AnomTest s;
	ASSERT_EQ("cinamo::(anonymous namespace)::AnomTest", demangle(s));
}
}

}

