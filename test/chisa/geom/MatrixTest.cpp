/*
 * MatrixTest.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: psi
 */

#include "../../TestCommon.h"
#include "../../../src/chisa/geom/Matrix.h"
#include <math.h>

namespace chisa {
namespace geom {

class MatrixTest : public ::testing::Test
{
protected:
public:
	void SetUp(){
	}
	void TearDown(){
	}

};

#define AssertIdentity(mat)\
for(int x=0;x<4;++x){\
	for(int y=0;y<4;++y){\
		ASSERT_FLOAT_EQ(x==y ? 1.0f : 0.0f, mat(x,y));\
	}\
}

TEST_F(MatrixTest, IdentityTest)
{
	AssertIdentity(Matrix());
}

TEST_F(MatrixTest, BasicTest)
{
	Matrix mat(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
	);
	mat = mat * mat;
	AssertIdentity(mat);

	mat*=mat;
	AssertIdentity(mat);
}

TEST_F(MatrixTest, FloatMultTest)
{
	Matrix mat(
			 1, 2, 3, 4,
			11,12,13,14,
			21,22,23,24,
			31,32,33,34
	);
	for(int x=0;x<4;++x){
		for(int y=0;y<4;++y){
			ASSERT_FLOAT_EQ(x+1+(y*10),mat(x,y));
		}
	}
	Matrix mat2(mat*2);
	for(int x=0;x<4;++x){
		for(int y=0;y<4;++y){
			ASSERT_FLOAT_EQ((x+1+(y*10))*2,mat2(x,y)); //２倍されてる
			ASSERT_FLOAT_EQ((x+1+(y*10)),mat(x,y)); //元のは変わらない
		}
	}

	mat = mat*1;
	for(int x=0;x<4;++x){
		for(int y=0;y<4;++y){
			ASSERT_FLOAT_EQ((x+1+(y*10)),mat(x,y));
		}
	}
	mat = mat/10;
	for(int x=0;x<4;++x){
		for(int y=0;y<4;++y){
			ASSERT_FLOAT_EQ((x+1+(y*10))/10.0f,mat(x,y));
		}
	}
}

TEST_F(MatrixTest, MultTest)
{
	Matrix mat(
			0, 0, 0, 1,
			0, 0, 1, 0,
			0, 1, 0, 0,
			1, 0, 0, 0
	);
	Matrix mat2(mat*mat);
	AssertIdentity(mat2);
}

}}


