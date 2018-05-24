/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#include "../TestCommon.h"
#include <cinamo/Matrix.h>

#include <cfloat>
#include <cmath>

namespace cinamo {

TEST(MatrixTest, InitializeTest)
{
	Mat<3,3> const mat = {
			1.0,2.0,3.0,
			4.0,5.0,6.0,
			7.0,8.0,9.0};
	ASSERT_FLOAT_EQ( 1.0, mat(0,0) );
	ASSERT_FLOAT_EQ( 6.0, mat(1,2) );

	Mat<3,3> const m;
	for(int i=0;i<3;++i){
		for(int j=0;j<3;++j){
			ASSERT_FLOAT_EQ( 0, m(i,j) );
		}
	}
}
TEST(MatrixTest, MultSameSizeMatrixTest)
{
	Mat<3,3> const m = {
			1.0, 1.0, 1.0,
			0.0, 0.0, 0.0,
			0.0, 0.0, 0.0,
	};
	Mat<3,3> const mat = {
			1.0,2.0,3.0,
			4.0,5.0,6.0,
			7.0,8.0,9.0};
	auto m2 = mat * m;
	ASSERT_FLOAT_EQ( 1.0, m2(0,0) );
	ASSERT_FLOAT_EQ( 1.0, m2(0,1) );
	ASSERT_FLOAT_EQ( 1.0, m2(0,2) );

	ASSERT_FLOAT_EQ( 4.0, m2(1,0) );
	ASSERT_FLOAT_EQ( 4.0, m2(1,1) );
	ASSERT_FLOAT_EQ( 4.0, m2(1,2) );

	ASSERT_FLOAT_EQ( 7.0, m2(2,0) );
	ASSERT_FLOAT_EQ( 7.0, m2(2,1) );
	ASSERT_FLOAT_EQ( 7.0, m2(2,2) );

	ASSERT_FLOAT_EQ(0, m.det());
}

TEST(MatrixTest, MultDifferentSizeMatrixTest)
{
	Mat<3,3> const mat = {
			1.0,2.0,3.0,
			4.0,5.0,6.0,
			7.0,8.0,9.0};
	Mat<3,1> const m = {
			1.0,
			1.0,
			1.0,
	};
	Mat<3,1> m2 = mat * m;
	ASSERT_FLOAT_EQ( 6.0, m2(0,0) );
	ASSERT_FLOAT_EQ( 15.0, m2(1,0) );
	ASSERT_FLOAT_EQ( 24.0, m2(2,0) );
}

TEST(MatrixTest, MultScalarTest)
{
	Mat<3,3> const mat = {
			1.0,2.0,3.0,
			4.0,5.0,6.0,
			7.0,8.0,9.0};
	Mat<3,1> const m = {
			1.0,
			1.0,
			1.0,
	};
	Mat<3,3> mat2 = mat*2;
	Mat<3,1> m2 = m*2;
	for(int i=0;i<3;++i){
		for(int j=0;j<3;++j){
			ASSERT_FLOAT_EQ( mat(i,j)*2, mat2(i,j) );
		}
	}
	for(int i=0;i<3;++i){
		ASSERT_FLOAT_EQ( m(i,0)*2, m2(i,0) );
	}
}

TEST(MatrixTest, DetTest)
{
	Mat<1> const mat1 = {1.0};
	ASSERT_FLOAT_EQ(1, mat1.det());
	Mat<2,2> const mat2 = {
			1.0,2.0,
			7.0,8.0,};
	ASSERT_FLOAT_EQ(-6, mat2.det());
	Mat<3,3> const mat3 = {
			1.0,0.0,1.0,
			3.0,2.0,4.0,
			1.0,6.0,1.0};
	ASSERT_FLOAT_EQ(-6, mat3.det());
}

}
