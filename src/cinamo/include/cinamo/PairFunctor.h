/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <utility>
#include <functional>
#include "Common.h"

namespace cinamo {

template <typename T, typename U, typename Order=std::less<T> >
struct PairCompare : public std::binary_function<std::pair<T,U>,std::pair<T,U>,bool>{
	bool operator()(T const& a, std::pair<T,U> const& b){
		return Order()(a, b.first);
	}
	bool operator()(std::pair<T,U> const& a, std::pair<T,U> const& b){
		return Order()(a.first, b.first);
	}
	bool operator()(std::pair<T,U> const& a, T const& b){
		return Order()(a.first, b);
	}
};

template <typename T, typename U, typename Eq=std::equal_to<T> >
struct PairEq : public std::unary_function<std::pair<T,U>,bool>{
private:
	T const& t_;
public:
	PairEq(T const& t):t_(t){}
	bool operator()(T const& a){
		return Eq()(a,t_);
	}
	bool operator()(std::pair<T,U> const& a){
		return Eq()(a.first,t_);
	}
};

}
