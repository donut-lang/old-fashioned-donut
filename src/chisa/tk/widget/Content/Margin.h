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
#include "../../../util/ClassUtil.h"
#include <cstddef>

namespace chisa {
namespace tk {
namespace widget {

class Margin {
	DEFINE_MEMBER(public, public, float, top);
	DEFINE_MEMBER(public, public, float, bottom);
	DEFINE_MEMBER(public, public, float, left);
	DEFINE_MEMBER(public, public, float, right);
public:
	Margin() noexcept :top_(0),bottom_(0),left_(0),right_(0){};
	Margin(const Margin& other) noexcept:
		top_(other.top_),bottom_(other.bottom_),left_(other.left_),right_(other.right_){};
	Margin& operator=(const Margin& other) noexcept{
		this->top_ = other.top_;
		this->bottom_ = other.bottom_;
		this->left_ = other.left_;
		this->right_ = other.right_;
		return *this;
	};
	Margin& operator=(const float& other) noexcept{
		this->top_ = other;
		this->bottom_ = other;
		this->left_ = other;
		this->right_ = other;
		return *this;
	};
	void margin(const float& other) noexcept{
		(*this) = other;
	};
	~Margin() noexcept = default;
private:
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
};

}}}
