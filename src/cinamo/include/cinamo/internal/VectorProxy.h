/* coding: utf-8 */
/**
 * Cinamo
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include "../ClassUtil.h"

namespace cinamo {

template <typename T>
class VectorProxy {
	STACK_OBJECT(VectorProxy);
	typedef std::vector<T> type;
	std::vector<char>& val;
public:
	VectorProxy(std::vector<char>& t):val(t){}
	VectorProxy(VectorProxy const&) = default;
	VectorProxy(VectorProxy&&) = default;
	VectorProxy& operator=(VectorProxy const&) = delete;
	VectorProxy& operator=(VectorProxy&&) = delete;
	~VectorProxy() = default;
	operator T() const {
		return type(this->val);
	}
	EnumProxy<T>& operator=(type const& t) {
		val.assign(reinterpret_cast<char const*>(&t[0]), reinterpret_cast<char const*>(&t[t.size()]));
		return *this;
	}
public:
	operator type() const{
		return type(reinterpret_cast<typename type::pointer>(&val[0]), reinterpret_cast<typename type::pointer>(&val[val.size()]));
	}
public:
	auto size() const noexcept -> decltype(val.size()) { return val.size(); };
	auto max_size() const noexcept -> decltype(val.max_size()) { return val.size(); };
	auto capacity() const noexcept -> decltype(val.capacity()) { return val.capacity(); };
	auto empty() const noexcept -> decltype(val.empty()) { return val.empty(); };
	auto resize(typename std::vector<char>::size_type t) -> decltype(val.resize(t)) { return val.resize(t); };
	template <typename U>
	auto resize(typename std::vector<char>::size_type t, U const& u) -> decltype(val.resize(t,u)) { return val.resize(t,u); };
	auto reserve(typename std::vector<char>::size_type t) -> decltype(val.reserve(t)) { return val.reserve(t); };
	auto shrink_to_fit() -> decltype(val.shrink_to_fit()) { return val.shrink_to_fit(); };
public:
	typename type::iterator begin() noexcept {
		return reinterpret_cast<typename type::iterator>(val.begin());
	}
	typename type::iterator end() noexcept {
		return reinterpret_cast<typename type::iterator>(val.end());
	}
	typename type::const_iterator cbegin() noexcept {
		return reinterpret_cast<typename type::const_iterator>(val.cbegin());
	}
	typename type::const_iterator cend() noexcept {
		return reinterpret_cast<typename type::const_iterator>(val.cend());
	}
public:
	typename type::reference operator[](typename std::vector<char>::size_type t) {
		return reinterpret_cast<typename type::reference>(val[t]);
	};
	typename type::const_reference operator[](typename std::vector<char>::size_type t) const {
		return reinterpret_cast<typename type::const_reference>(val[t]);
	}
	typename type::reference at(typename std::vector<char>::size_type t) {
		return reinterpret_cast<typename type::reference>(val.at(t));
	}
	typename type::const_reference at(typename std::vector<char>::size_type t) const {
		return reinterpret_cast<typename type::const_reference>(val.at(t));
	}
	typename type::pointer data() noexcept {
		return reinterpret_cast<typename type::pointer>(val.data());
	}
	typename type::const_pointer data() const noexcept {
		return reinterpret_cast<typename type::const_pointer>(val.data());
	}
	typename type::reference front() noexcept {
		return reinterpret_cast<typename type::reference>(val.front());
	}
	typename type::const_reference front() const noexcept {
		return reinterpret_cast<typename type::const_reference>(val.front());
	}
	typename type::reference back() noexcept {
		return reinterpret_cast<typename type::reference>(val.back());
	}
	typename type::const_reference back() const noexcept {
		return reinterpret_cast<typename type::const_reference>(val.back());
	}
public:
	template <class InputIterator> auto assign(InputIterator first, InputIterator last) -> decltype(val.assign(first,last)) { return val.assign(first,last); }
	auto assign(typename type::size_type n, const T& u) -> decltype(val.assign(n,u)) { return val.assign(n, u); }
	auto push_back(const T& x) -> decltype(val.push_back(x)) { return val.push_back(x); }
	template <class... Args> auto emplace_back(Args&&... args) -> decltype(val.emplace_back(std::move(args)...)) { return val.emplace_back(std::move(args)...); }
	auto pop_back() -> decltype(val.pop_back()) { return val.pop_back(); }
	typename type::iterator insert(typename type::const_iterator position, const T& x) {
		return reinterpret_cast<typename type::iterator>( val.insert(reinterpret_cast<typename std::vector<char>::iterator>(position), x) );
	}
	typename type::iterator insert(typename type::const_iterator position, T&& x) {
		return reinterpret_cast<typename type::iterator>( val.insert(reinterpret_cast<typename std::vector<char>::iterator>(position), std::move(x)) );
	}
	typename type::iterator insert(typename type::const_iterator position, typename type::size_type n, const T& x) {
		return reinterpret_cast<typename type::iterator>( val.insert(reinterpret_cast<typename std::vector<char>::iterator>(position), n, x) );
	}
	template <class InputIterator> typename type::iterator insert(typename type::const_iterator position, InputIterator first, InputIterator last) {
		return reinterpret_cast<typename type::iterator>(val.insert(first,last));
	}
	template <class... Args>
	typename type::iterator emplace(typename type::const_iterator position, Args&&... args)
	{
		return reinterpret_cast<typename type::iterator>(val.emplace(reinterpret_cast<typename std::vector<char>::iterator>(position), std::move(args)...));
	}
	template <class... Args>
	void emplace_back(Args&&... args)
	{
		val.emplace_back(std::move(args)...);
	}
	typename type::iterator erase(typename type::const_iterator position)
	{
		return reinterpret_cast<typename type::iterator>(val.erase(reinterpret_cast<typename std::vector<char>::iterator>(position)));
	}
	typename type::iterator erase(typename type::const_iterator first, typename type::const_iterator last)
	{
		return reinterpret_cast<typename type::iterator>(val.erase(reinterpret_cast<typename std::vector<char>::iterator>(first), reinterpret_cast<typename std::vector<char>::iterator>(last)));
	}
	auto clear() -> decltype(val.clear()){ return val.clear(); }
};

template <typename T>
class ConstVectorProxy {
	STACK_OBJECT(ConstVectorProxy);
	typedef std::vector<T> type;
	std::vector<char> const& val;
public:
	ConstVectorProxy(std::vector<char> const& t):val(t){}
	ConstVectorProxy(ConstVectorProxy const&) = default;
	ConstVectorProxy(ConstVectorProxy&&) = default;
	ConstVectorProxy& operator=(ConstVectorProxy const&) = delete;
	ConstVectorProxy& operator=(ConstVectorProxy&&) = delete;
	~ConstVectorProxy() = default;
	operator T() const {
		return type(this->val);
	}
public:
	operator type() const{
		return type(reinterpret_cast<typename type::const_pointer>(&val[0]), reinterpret_cast<typename type::const_pointer>(&val[val.size()]));
	}
public:
	auto size() const noexcept -> decltype(val.size()) { return val.size(); };
	auto max_size() const noexcept -> decltype(val.max_size()) { return val.size(); };
	auto capacity() const noexcept -> decltype(val.capacity()) { return val.capacity(); };
	auto empty() const noexcept -> decltype(val.empty()) { return val.empty(); };
public:
	typename type::const_iterator cbegin() noexcept {
		return reinterpret_cast<typename type::const_iterator>(val.cbegin());
	}
	typename type::const_iterator cend() noexcept {
		return reinterpret_cast<typename type::const_iterator>(val.cend());
	}
public:
	typename type::const_reference operator[](typename std::vector<char>::size_type t) const {
		return reinterpret_cast<typename type::const_reference>(val[t]);
	}
	typename type::const_reference at(typename std::vector<char>::size_type t) const {
		return reinterpret_cast<typename type::const_reference>(val.at(t));
	}
	typename type::const_pointer data() const noexcept {
		return reinterpret_cast<typename type::const_pointer>(val.data());
	}
	typename type::const_reference front() const noexcept {
		return reinterpret_cast<typename type::const_reference>(val.front());
	}
	typename type::const_reference back() const noexcept {
		return reinterpret_cast<typename type::const_reference>(val.back());
	}
};

}
