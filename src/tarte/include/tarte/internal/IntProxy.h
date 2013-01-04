/**
 * Tarte
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "../ClassUtil.h"

namespace tarte {


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
