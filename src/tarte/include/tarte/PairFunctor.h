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
#include <utility>
#include <functional>
#include "Common.h"

namespace tarte {

template <typename T, typename U>
struct PairCompare : public std::binary_function<std::pair<T,U>,std::pair<T,U>,bool>{
	bool operator()(T const& a, std::pair<T,U> const& b){
		return std::less<T>()(a, b.first);
	}
	bool operator()(std::pair<T,U> const& a, std::pair<T,U> const& b){
		return std::less<T>()(a.first, b.first);
	}
	bool operator()(std::pair<T,U> const& a, T const& b){
		return std::less<T>()(a.first, b);
	}
};

template <typename T, typename U>
struct PairEq : public std::unary_function<std::pair<T,U>,bool>{
private:
	T const& t_;
public:
	PairEq(T const& t):t_(t){}
	bool operator()(T const& a){
		return a == t_;
	}
	bool operator()(std::pair<T,U> const& a){
		return a.first == t_;
	}
};

}

#undef DEF_OVERLOD
