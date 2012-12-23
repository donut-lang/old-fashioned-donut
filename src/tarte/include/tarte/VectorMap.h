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
#include <vector>
#include <algorithm>
#include "PairFunctor.h"

namespace tarte {

template <typename T, typename U>
class VectorMap {
public:
	typedef PairCompare<T,U> Comparator;
	typedef std::vector<std::pair<T,U> > Vector;
	typedef std::pair<T,U> Pair;
	typedef typename Vector::iterator Iterator;
	typedef typename Vector::const_iterator ConstIterator;
	typedef typename Vector::size_type SizeType;
private:
	Vector data_;
	inline bool eq(T const& key, Pair const& p) const noexcept {
		Comparator cmp;
		return !(cmp(p, key) || cmp(key, p));
	}
public:
	VectorMap(unsigned int reserved):data_(){ data_.reserve(reserved); };
	VectorMap(){};
	VectorMap& operator=(VectorMap const& other) = default;
	VectorMap& operator=(VectorMap && other) = default;
	VectorMap(VectorMap const& other) = default;
	VectorMap(VectorMap && other) = default;
	~VectorMap() noexcept = default;
	inline Iterator begin() noexcept { return data_.begin(); }
	inline Iterator end() noexcept { return data_.end(); }
	inline ConstIterator begin() const noexcept { return data_.begin(); }
	inline ConstIterator end() const noexcept { return data_.end(); }
	inline ConstIterator cbegin() const noexcept { return data_.cbegin(); }
	inline ConstIterator cend() const noexcept { return data_.cend(); }
public:
	inline Iterator find( T const& key ) {
		Comparator cmp;
		auto it = std::lower_bound( data_.begin(), data_.end(), key, cmp );
		if( it == data_.end() || !eq(key, *it) ) {
			return data_.end();
		}else{
			return it;
		}
	}
	inline ConstIterator find( T const& key ) const {
		Comparator cmp;
		auto it = std::lower_bound( data_.begin(), data_.end(), key, cmp );
		if( it == data_.end() || !eq(key, *it) ) {
			return data_.end();
		}else{
			return it;
		}
	}
	inline bool have (T const& key) const {
		return this->end() != find(key);
	}
	inline bool insert( T const& key, U const& val ) {
		auto it = std::lower_bound( data_.begin(), data_.end(), key, Comparator() );
		if( it == data_.end() || !eq(key, *it) ) {
			data_.insert(it, Pair(key, val));
			return true;
		}else{
			return false;
		}
	}
	inline bool update( T const& key, U const& val ) {
		Comparator cmp;
		auto it = std::lower_bound( data_.begin(), data_.end(), key, cmp );
		if( it == data_.end() || !eq(key, *it) ) {
			data_.insert(it, Pair(key, val));
			return false;
		}else{
			it->second = val;
			return true;
		}
	}
	inline bool remove(T const& key) {
		Comparator cmp;
		auto it = find(key);
		if( it == end() ) {
			return false;
		}else{
			this->data_.erase(it);
			return true;
		}
	}
	inline Iterator erase(Iterator it)
{
		return data_.erase(it);
	}
	inline void clear() noexcept {
		this->data_.clear();
	}
	inline SizeType size() const noexcept{ return data_.size(); };
	inline bool empty() const noexcept{ return data_.empty(); };
};

}

#undef DEF_OVERLOD
