/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */
#pragma once
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <array>
#include <cinamo/ClassUtil.h>

namespace cinamo {

template <size_t N, size_t M=N, bool id=N==M>
class Mat;

template <size_t N, size_t M, typename Self>
class MatCommon
{
private:
	std::array<double, N*M> val_;
private:
	MatCommon(std::array<double, N*M>&& val):val_(val){};
public:
	MatCommon():val_(){};
	MatCommon(MatCommon<N,M, Self> const& o):val_(o.val_){};
	MatCommon(MatCommon<N,M, Self>&& o):val_(std::move(o.val_)){};

	MatCommon<N,M, Self>& operator=(MatCommon<N,M,Self> const& o){
		val_ = o.val_;
		return *this;
	};
	MatCommon<N,M,Self>& operator=(MatCommon<N,M,Self>&& o){
		val_ = std::move(o.val_);
		return *this;
	}
public:
	template<typename ...E>
	constexpr MatCommon(double const& d, E const& ...e)
	:val_{{d, e...}}{
		static_assert((1+sizeof...(E) == N*M), "size not match");
	}
	~MatCommon() = default;
public:
	double const& operator()(size_t i, size_t j) const{
		assert(i >= 0 && i< N);
		assert(j >= 0 && j< M);
		return val_[i*M+j];
	}
	double& operator()(size_t i, size_t j){
		assert(i >= 0 && i< N);
		assert(j >= 0 && j< M);
		return val_[i*M+j];
	}
	Self& operator *=(double const& o) {
		for(double &d : val_){
			d *= o;
		}
		return static_cast<Self&>(*this);
	}
	Self operator *(double const& o) const{
		Self self(*this);
		self *= o;
		return self;
	}
	// 同じ次元の正方行列
	template <size_t N_, size_t M_, typename Self_>
	auto operator *=(MatCommon<N_,M_, Self_> const& o)
	-> typename std::enable_if<M_==M && std::is_same<Self,Self_>::value, Self& >::type
	{
		for(size_t i=0;i<N;++i) {
			for(size_t j=0;j<M_;++j) {
				double sum = 0;
				for(size_t k=0;k<M;++k){
					sum += ((*this)(i, k))*(o(k,j));
				}
				(*this)(i,j)=sum;
			}
		}
		return static_cast<Self&>(*this);
	}
	// 同じ次元の正方行列
	template <size_t N_, size_t M_, typename Self_>
	auto operator *(MatCommon<N_,M_,Self_> const& o) const
	-> typename std::enable_if<N_==N&&M_==M&&N==M&&std::is_same<Self,Self_>::value,Self >::type
	{
		Self self(*this);
		self*=o;
		return std::move(self);
	}
	// そうでない
	template <size_t L, typename Self_>
	auto operator *(MatCommon<M, L,Self_> const& o) const
	-> typename std::enable_if<N!=M || L!=M, Mat<N,L> >::type
	{
		Mat<N,L> val;
		for(size_t i=0;i<N;++i) {
			for(size_t j=0;j<L;++j) {
				double sum = 0;
				for(size_t k=0;k<M;++k){
					sum += ((*this)(i, k))*o(k,j);
				}
				val(i,j)=sum;
			}
		}
		return val;
	}
	Self& operator +=(Mat<N,M> const& o)
	{
		for(size_t i=0;i<N;++i) {
			for(size_t j=0;j<M;++j) {
				(*this)(i,j)+=o(i,j);
			}
		}
		return static_cast<Self&>(*this);
	}

	Mat<N,M> operator +(Mat<N,M> const& o) const{
		Mat<N, M> self(*this);
		return std::move(self += o);
	}

	Self& rowAdd(size_t from, size_t to,double const& mult){
		for(size_t j=0;j<M;++j){
			(*this)(to,j) += (*this)(from, j);
		}
		return static_cast<Self&>(*this);
	}
	Self createTriangle() const{
		Self self(*this);
		return self;
	}
};

template <size_t N>
class Mat<N,N,true> final : public MatCommon<N,N, Mat<N,N,true> >{
public:
	Mat(Mat<N,N,true> const& other) = default;
	Mat(Mat<N,N,true>&& other) = default;
	Mat<N,N,true>& operator=(Mat<N,N,true> const& other) = default;
	Mat<N,N,true>& operator=(Mat<N,N,true>&& other) = default;
	template<typename ...E>
	constexpr Mat(E&& ...e)
	:MatCommon<N,N, Mat<N,N,true> >(std::forward<E>(e)...){
	}
public:
	double det() const{
		return 0;
	}
};

template<>
inline double Mat<1,1,true>::det() const{
	return (*this)(0,0);
}
template<>
inline double Mat<2,2,true>::det() const{
	return ((*this)(0,0)*(*this)(1,1))-((*this)(0,1)*(*this)(1,0));
}
template<>
inline double Mat<3,3,true>::det() const{
	//サラスの方法
	return
			 ((*this)(0,0)*(*this)(1,1)*(*this)(2,2))
			+((*this)(0,1)*(*this)(1,2)*(*this)(2,0))
			+((*this)(0,2)*(*this)(1,0)*(*this)(2,1))
			-((*this)(0,2)*(*this)(1,1)*(*this)(2,0))
			-((*this)(1,2)*(*this)(2,1)*(*this)(0,0))
			-((*this)(2,2)*(*this)(0,1)*(*this)(1,0));
}

template <size_t N, size_t M>
class Mat<N,M,false> final : public MatCommon<N,M, Mat<N,M,false> >{
public:
	Mat(Mat<N,M,false> const& other) = default;
	Mat(Mat<N,M,false>&& other) = default;
	Mat<N,M,false>& operator=(Mat<N,M,false> const& other) = default;
	Mat<N,M,false>& operator=(Mat<N,M,false>&& other) = default;
	template<typename ...E>
	constexpr Mat(E&& ...e)
	:MatCommon<N,M, Mat<N,M,false> >(std::forward<E>(e)...){
	}
};

}
