/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include "../ClassUtil.h"

namespace cinamo {


template <typename T>
class IntProxy {
	STACK_OBJECT(IntProxy);
	typedef T type;
	int& val;
public:
	IntProxy(int& t):val(t){}
	IntProxy(IntProxy const&) = default;
	IntProxy(IntProxy&&) = default;
	IntProxy& operator=(IntProxy const&) = delete;
	IntProxy& operator=(IntProxy&&) = delete;
	~IntProxy() = default;
	operator T() const {
		return T(this->val);
	}
	IntProxy<T>& operator=(T const& t) {
		this->val = static_cast<int>(t);
		return *this;
	}
};

template <typename T>
class ConstIntProxy {
	STACK_OBJECT(ConstIntProxy);
	typedef T type;
	const int& val;
public:
	ConstIntProxy(int const& t):val(t){}
	ConstIntProxy(ConstIntProxy const&) = default;
	ConstIntProxy(ConstIntProxy&&) = default;
	ConstIntProxy& operator=(ConstIntProxy const&) = delete;
	ConstIntProxy& operator=(ConstIntProxy&&) = delete;
	~ConstIntProxy() = default;
	operator T() const {
		return T(this->val);
	}
};

// TODO: 若干繰り返しが面倒くさいけど繰り返すのが楽なのでは
template <typename T>
class UnsignedIntProxy {
	STACK_OBJECT(UnsignedIntProxy);
	typedef T type;
	unsigned int& val;
public:
	UnsignedIntProxy(unsigned int& t):val(t){}
	UnsignedIntProxy(UnsignedIntProxy const&) = default;
	UnsignedIntProxy(UnsignedIntProxy&&) = default;
	UnsignedIntProxy& operator=(UnsignedIntProxy const&) = delete;
	UnsignedIntProxy& operator=(UnsignedIntProxy&&) = delete;
	~UnsignedIntProxy() = default;
	operator T() const {
		return T(this->val);
	}
	UnsignedIntProxy<T>& operator=(T const& t) {
		this->val = static_cast<int>(t);
		return *this;
	}
};

template <typename T>
class ConstUnsignedIntProxy {
	STACK_OBJECT(ConstUnsignedIntProxy);
	typedef T type;
	const unsigned& val;
public:
	ConstUnsignedIntProxy(unsigned const& t):val(t){}
	ConstUnsignedIntProxy(ConstUnsignedIntProxy const&) = default;
	ConstUnsignedIntProxy(ConstUnsignedIntProxy&&) = default;
	ConstUnsignedIntProxy& operator=(ConstUnsignedIntProxy const&) = delete;
	ConstUnsignedIntProxy& operator=(ConstUnsignedIntProxy&&) = delete;
	~ConstUnsignedIntProxy() = default;
	operator T() const {
		return T(this->val);
	}
};

}
