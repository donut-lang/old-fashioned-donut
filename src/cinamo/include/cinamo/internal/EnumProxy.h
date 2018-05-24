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
class EnumProxy {
	STACK_OBJECT(EnumProxy);
	typedef T type;
	int& val;
public:
	EnumProxy(int& t):val(t){}
	EnumProxy(EnumProxy const&) = default;
	EnumProxy(EnumProxy&&) = default;
	EnumProxy& operator=(EnumProxy const&) = delete;
	EnumProxy& operator=(EnumProxy&&) = delete;
	~EnumProxy() = default;
	operator T() const {
		return T(this->val);
	}
	EnumProxy<T>& operator=(T const& t) {
		this->val = static_cast<int>(t);
		return *this;
	}
};

template <typename T>
class ConstEnumProxy {
	STACK_OBJECT(ConstEnumProxy);
	typedef T type;
	const int& val;
public:
	ConstEnumProxy(int const& t):val(t){}
	ConstEnumProxy(ConstEnumProxy const&) = default;
	ConstEnumProxy(ConstEnumProxy&&) = default;
	ConstEnumProxy& operator=(ConstEnumProxy const&) = delete;
	ConstEnumProxy& operator=(ConstEnumProxy&&) = delete;
	~ConstEnumProxy() = default;
	operator T() const {
		return T(this->val);
	}
};

}
