/**
 * Chisa
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

namespace chisa {
namespace util {

class XArchiver {
	DISABLE_COPY_AND_ASSIGN(XArchiver);
private:
	template <typename T>
	class HasSerializer {
		template <typename U>
		static auto check(U u) -> decltype(u.serialize(std::declval<XArchiver&>()), std::true_type());
		static auto check(...) -> decltype( std::false_type() );
	public:
		typedef decltype(check(std::declval<T>())) check_func_decl;
		static const bool value = check_func_decl::value;
	};
private:
	Handler<XArray> array_;
	bool decode_now_;
	size_t count_;
private:
	explicit XArchiver(Handler<XArray> const& a):array_(a), decode_now_(true), count_(0){}
public:
	explicit XArchiver();
	explicit XArchiver(XValue const& val);
public:
	XValue data() {
		return array_;
	}
public:
	template <typename T> XArchiver& operator &(T& val);
	template <typename T> XArchiver& operator>>(T& t){
		return this->operator &(t);
	}
	template <typename T> XArchiver& operator<<(T& t){
		return this->operator &(t);
	}
public:
	template <typename T> inline static auto deserialize(T& val, XValue const& xval) -> typename std::enable_if< HasSerializer<T>::value, void >::type;
	template <typename T> inline static auto serialize(T& val) -> typename std::enable_if< HasSerializer<T>::value, XValue >::type;
	template <typename T> static auto deserialize(T& val, XValue const& xval) -> typename std::enable_if<!HasSerializer<T>::value, void >::type;
	template <typename T> static auto serialize(T& val) -> typename std::enable_if<!HasSerializer<T>::value, XValue >::type;
};

}}

#include "internal/XArchiver.h"
