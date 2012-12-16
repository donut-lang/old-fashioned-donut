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
	const bool decode_now_;
	Handler<XArray> array_;
	size_t array_index_;
protected:
	explicit XArchiver(bool decode_now);
	explicit XArchiver(Handler<XArray> const& a);
	explicit XArchiver() = delete;
	virtual ~XArchiver() noexcept = default;
protected:
	inline size_t index() const noexcept { return this->array_index_; };
	inline void index( size_t const& s ) noexcept { this->array_index_ = s; };
	inline Handler<XArray>& array() noexcept { return this->array_; };
	inline Handler<XArray>& array( Handler<XArray> const& a ) noexcept { return this->array_ = a; };
public:
	template <typename T> XArchiver& operator &(T& val);
};

class XArchiverIn : public XArchiver {
public:
	explicit XArchiverIn();
	explicit XArchiverIn(XValue const& v);
	explicit XArchiverIn(Handler<XArray> const& a):XArchiver(a){}
public:
	inline XArchiver& operator<<(XValue const& val){
		this->array(val.as<XArray>());
		this->index(0);
		return *this;
	}
	template <typename T> inline XArchiver& operator>>(T& t){
		if( !this->array() ){
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Please load xvalue first.");
		}
		this->operator &(t);
		this->array(Handler<XArray>());
		this->index(0);
		return *this;
	}
};

class XArchiverOut : public XArchiver {
public:
	explicit XArchiverOut();
public:
	inline XArchiver& operator>>(XValue& val){
		if( !this->array() ){
			throw logging::Exception(__FILE__, __LINE__, "[BUG] Please serialize objects first.");
		}
		val = this->array();
		this->array(Handler<XArray>());
		this->index(0);
		return *this;
	}
	template <typename T> inline XArchiver& operator<<(T& t){
		if( !this->array() ){
			this->array(Handler<XArray>(new XArray));
			this->index(0);
		}
		return this->operator &(t);
	}
};

// シリアライズするオブジェクトが、serializeメソッドを持っているかどうかチェックする
template <typename T>
class HasSerializer {
	template <typename U>
	static auto check(U u) -> decltype(u.serialize(std::declval<XArchiver&>()), std::true_type());
	static auto check(...) -> decltype( std::false_type() );
public:
	typedef decltype(check(std::declval<T>())) check_func_decl;
	static const bool value = check_func_decl::value;
};

//持っている場合は再帰的に適用する
template <typename T, bool has_serializer=HasSerializer<T>::value >
struct XSerializer {
	static XValue serialize(T& val){
		XArchiverOut a;
		XValue v;
		val.serialize(a);
		a >> v;
		return v;
	}
	static void deserialize(T& val, XValue const& xval){
		XArchiverIn arc(xval.as<XArray>());
		val.serialize(arc);
	}
};

//持ってない場合は…誰かが定義する
template <typename T>
struct XSerializer<T,false > {
	static XValue serialize(T& val);
	static void deserialize(T& val, XValue const& xval);
};

}}

#include "internal/XArchiver.h"
