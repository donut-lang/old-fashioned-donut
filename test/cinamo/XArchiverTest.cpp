/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/XArchiver.h>
#include <math.h>

namespace cinamo {
namespace {

struct BasicTest{
	int x,y,z;
	void serialize(XArchiver& arc){
		arc & x & y & z;
	}
};

struct ComplexTest{
	int x,y,z;
	BasicTest inner;
	void serialize(XArchiver& arc){
		arc & x & y & z & inner;
	}
};

struct EnumTest{
	enum ENUM{
		X,
		Y,
		Z
	} en;
	void serialize(XArchiver& arc){
		arc & en;
	}
};

TEST(XArchiveTest, BasicStructTest)
{
	XValue v;
	{
		BasicTest t;
		t.x=1;
		t.y=2;
		t.z=3;
		XArchiverOut arc;
		arc << t;
		arc >> v;
	}
	{
		BasicTest t;
		t.x=0;
		t.y=0;
		t.z=0;
		XArchiverIn arc(v);
		arc >> t;
		ASSERT_EQ(t.x, 1);
		ASSERT_EQ(t.y, 2);
		ASSERT_EQ(t.z, 3);
	}
}

TEST(XArchiveTest, ComplexStructTest)
{
	XValue v;
	{
		ComplexTest t;
		t.x=1;
		t.y=2;
		t.z=3;
		t.inner.x=4;
		t.inner.y=5;
		t.inner.z=6;
		XArchiverOut arc;
		arc << t;
		arc >> v;
		ASSERT_ANY_THROW(arc >> v); //二回同じ操作はできない
	}
	{
		ComplexTest t;
		t.x=0;
		t.y=0;
		t.z=0;
		XArchiverIn arc(v);
		arc >> t;
		ASSERT_ANY_THROW(arc >> t); //二回同じ操作はできない
		ASSERT_EQ(t.x, 1);
		ASSERT_EQ(t.y, 2);
		ASSERT_EQ(t.z, 3);
		ASSERT_EQ(t.inner.x, 4);
		ASSERT_EQ(t.inner.y, 5);
		ASSERT_EQ(t.inner.z, 6);
	}
}

TEST(XArchiveTest, VectorTest)
{
	std::vector<int> a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	XValue v;
	{
		XArchiverOut out;
		out << a;
		out >> v;
	}
	std::vector<int> b;
	{
		XArchiverIn in;
		in << v;
		in >> b;
	}
	ASSERT_EQ(a,b);
}

TEST(XArchiveTest, ArrayTest)
{
	int a[3]={1,2,3};
	XValue v;
	{
		XArchiverOut out;
		out << a;
		out >> v;
	}
	int b[3];
	{
		XArchiverIn in;
		in << v;
		in >> b;
	}
	ASSERT_TRUE(ArraysMatch(a,b));
}

TEST(XArchiveTest, EnumTest)
{
	EnumTest en;
	en.en=EnumTest::ENUM::Z;
	XValue v;
	{
		XArchiverOut out;
		out << en;
		out >> v;
	}
	EnumTest eb;
	{
		XArchiverIn in;
		in << v;
		in >> eb;
	}
	ASSERT_EQ(EnumTest::ENUM::Z, eb.en);
}

TEST(XArchiveTest, BinaryHeapTest)
{
	char* data( new char[256] );
	char* data2( new char[256] );
	for(int i=0;i<256;++i){
		data[i] = i;
		data2[i] = i;
	}
	XValue v;
	{
		XArchiverOut out;
		out.binary(data, 256);
		out >> v;
	}
	std::memset(data, 256, 0xff);
	{
		XArchiverIn in;
		in << v;
		in.binary(data, 256);
	}
	for(int i=0;i<256;++i) {
		ASSERT_EQ(data2[i], data[i]);
	}
	delete [] data;
	delete [] data2;
}

TEST(XArchiveTest, BinaryArrayTest)
{
	char data[256];
	char data2[256];
	for(int i=0;i<256;++i){
		data[i] = i;
		data2[i]=i;
	}
	XValue v;
	{
		XArchiverOut out;
		out << data;
		out >> v;
	}
	std::memset(data, 256, 0xff);
	{
		XArchiverIn in;
		in << v;
		in >> data;
	}
	ASSERT_TRUE(ArraysMatch(data2, data));
}

TEST(XArchiveTest, UnsignedBinaryArrayTest)
{
	unsigned char data[256];
	unsigned char data2[256];
	for(int i=0;i<256;++i){
		data[i] = i;
		data2[i]=i;
	}
	XValue v;
	{
		XArchiverOut out;
		out << data;
		out >> v;
	}
	std::memset(data, 256, 0xff);
	{
		XArchiverIn in;
		in << v;
		in >> data;
	}
	ASSERT_TRUE(ArraysMatch(data2, data));
}

struct SaveLoadSample {
	int x,y,z;
	void save(XArchiverOut& out) {
		out & x & y & z;
	}
	void load(XArchiver& in) {
		in & x & y & z;
	}
};

TEST(XArchiveTest, SaveLoadTest)
{
	SaveLoadSample sample2;
	XValue v;
	{
		XArchiverOut out;
		SaveLoadSample sample;
		sample.x = 1;
		sample.y = 2;
		sample.z = 3;
		out << sample;
		out >> v;
	}
	{
		XArchiverIn in;
		in << v;
		in >> sample2;
	}
	ASSERT_EQ(1, sample2.x);
	ASSERT_EQ(2, sample2.y);
	ASSERT_EQ(3, sample2.z);
}

struct XSaveLoadSample {
	int x,y,z;
	XValue save() {
		Handler<XObject> obj(new XObject);
		obj->set("x", x);
		obj->set("y", y);
		obj->set("z", z);
		return obj;
	}
	void load(XValue const& in) {
		Handler<XObject> obj(in.as<XObject>());
		x = obj->get<int>("x");
		y = obj->get<int>("y");
		z = obj->get<int>("z");
	}
};

TEST(XArchiveTest, XSaveLoadTest)
{
	XSaveLoadSample sample2;
	XValue v;
	{
		XArchiverOut out;
		XSaveLoadSample sample;
		sample.x = 1;
		sample.y = 2;
		sample.z = 3;
		out << sample;
		out >> v;
	}
	{
		XArchiverIn in;
		in << v;
		in >> sample2;
	}
	ASSERT_EQ(1, sample2.x);
	ASSERT_EQ(2, sample2.y);
	ASSERT_EQ(3, sample2.z);
}

TEST(XArchiveTest, RawXValue)
{
	std::string src;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* e;
	{
		Handler<XObject> obj(new XObject);
		obj->set("1", 1);
		obj->set("2", 2);
		obj->set("3", 3);
		XArchiverOut out;
		out << obj;
		e = out.toXValue().toXML(&doc);
		doc.InsertFirstChild(e);
	}
	{
		XArchiverIn in(XValue::fromXML(e));
		Handler<XObject> obj;
		in >> obj;
		ASSERT_EQ(1, obj->get<decltype(1)>("1"));
		ASSERT_EQ(2, obj->get<decltype(2)>("2"));
		ASSERT_EQ(3, obj->get<decltype(3)>("3"));
	}

}

}}
