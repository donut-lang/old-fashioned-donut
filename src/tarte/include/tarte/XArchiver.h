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

namespace tarte {

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
	XArchiver& binary(char* const bin, std::size_t const& len);
	XArchiver& binary(unsigned char* const bin, std::size_t const& len){
		return binary(reinterpret_cast<char* const>(bin), len);
	}
};

class XArchiverIn : public XArchiver {
public:
	explicit XArchiverIn();
	explicit XArchiverIn(XValue const& v);
	explicit XArchiverIn(Handler<XArray> const& a):XArchiver(a){}
public:
	inline XArchiverIn& operator<<(XValue const& val){
		this->array(val.as<XArray>());
		this->index(0);
		return *this;
	}
	template <typename T> inline XArchiverIn& operator>>(T& t){
		if( !this->array() ){
			TARTE_EXCEPTION(Exception, "[BUG] Please load xvalue first.");
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
	inline XArchiverOut& operator>>(XValue& val){
		if( !this->array() ){
			TARTE_EXCEPTION(Exception, "[BUG] Please serialize objects first.");
		}
		val = this->array();
		this->array(Handler<XArray>());
		this->index(0);
		return *this;
	}
	template <typename T> inline XArchiverOut& operator<<(T& t){
		if( !this->array() ){
			this->array(Handler<XArray>(new XArray));
			this->index(0);
		}
		this->operator &(t);
		return *this;
	}
	XValue toXValue(){
		XValue v;
		(*this) >> v;
		return v;
	}
};

// シリアライズするオブジェクトが、serializeメソッドを持っているかどうかチェックする
template <typename T>
class HasSerializer {
	template <typename U>
	static auto check(U* u) -> decltype(u->serialize(std::declval<XArchiver&>()), std::true_type());
	static auto check(...) -> decltype( std::false_type() );
public:
	typedef decltype(check(std::declval<T*>())) check_func_decl;
	static const bool value = check_func_decl::value;
};

template <typename T>
class HasSaveLoad {
	template <typename U>
	static auto checkSave(U* u) -> decltype(u->save(std::declval<XArchiverOut&>()), std::true_type());
	static auto checkSave(...) -> decltype( std::false_type() );
	template <typename U>
	static auto checkLoad(U* u) -> decltype(u->load(std::declval<XArchiverIn&>()), std::true_type());
	static auto checkLoad(...) -> decltype( std::false_type() );
public:
	typedef decltype(checkSave(std::declval<T*>())) check_save_decl;
	typedef decltype(checkLoad(std::declval<T*>())) check_load_decl;
	static const bool value = check_save_decl::value && check_load_decl::value;
};

template <typename T>
class HasXValueSaveLoad {
	template <typename U>
	static auto checkSave(U* u) -> typename std::enable_if<std::is_same<decltype(u->save()), XValue>::value, std::true_type>::type;
	static auto checkSave(...) -> decltype( std::false_type() );
	template <typename U>
	static auto checkLoad(U* u) -> decltype(u->load(std::declval<XValue const&>()), std::true_type());
	static auto checkLoad(...) -> decltype( std::false_type() );
public:
	typedef decltype(checkSave(std::declval<T*>())) check_save_decl;
	typedef decltype(checkLoad(std::declval<T*>())) check_load_decl;
	static const bool value = check_save_decl::value && check_load_decl::value;
};

template <typename T,typename U=void>
struct XSerializer {
	static XValue serialize(T& val);
	static void deserialize(T& val, XValue const& xval);
};

//enumを持っている
template <typename T>
struct XSerializer<T, typename std::enable_if<std::is_enum<T>::value>::type> {
	static XValue serialize(T& val){
		return XValue(val);
	}
	static void deserialize(T& val, XValue const& xval){
		val = xval.as<T>();
	}
};

//ロードとセーブがカスタマイズされている場合はそれを利用する
// XArchiver利用
template <typename T>
struct XSerializer<T, typename std::enable_if<HasSaveLoad<T>::value>::type> {
	static XValue serialize(T& val){
		XArchiverOut a;
		XValue v;
		val.save(a);
		a >> v;
		return v;
	}
	static void deserialize(T& val, XValue const& xval){
		XArchiverIn arc(xval.as<XArray>());
		val.load(arc);
	}
};

//ロードとセーブがカスタマイズされている場合はそれを利用する
// XValue利用
template <typename T>
struct XSerializer<T, typename std::enable_if<HasXValueSaveLoad<T>::value>::type> {
	static XValue serialize(T& val){
		return val.save();
	}
	static void deserialize(T& val, XValue const& xval){
		val.load(xval);
	}
};

//独自のシリアライズ関数を持っている場合はそれを利用する
template <typename T>
struct XSerializer<T, typename std::enable_if<HasSerializer<T>::value>::type> {
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

}

#include "internal/XArchiver.h"
