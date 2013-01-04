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
#include "XVal.h"
#include "ClassUtil.h"

namespace tarte {

template <typename T>
class EnumProxy {
	STACK_OBJECT(EnumProxy);
	typedef T type;
	int& val;
public:
	EnumProxy(int& t):val(t){}
	EnumProxy(int const& t):val(reinterpret_cast<int&>(t)){}
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
