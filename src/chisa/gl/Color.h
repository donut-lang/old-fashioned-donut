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
#include "../util/ClassUtil.h"

namespace chisa {
namespace gl {

class Color {
	STACK_OBJECT(Color);
public:
	DEFINE_MEMBER(public, public, float, red);
	DEFINE_MEMBER(public, public, float, green);
	DEFINE_MEMBER(public, public, float, blue);
	DEFINE_MEMBER(public, public, float, alpha);
public:
	constexpr Color() noexcept:red_(0), green_(0), blue_(0), alpha_(0){};
	constexpr Color(float r, float g, float b, float a) noexcept:red_(r), green_(g), blue_(b), alpha_(a){};
	Color(const Color& other) = default;
	Color(Color&& other) = default;
	Color& operator=(const Color& other) = default;
	Color& operator=(Color&& other) = default;
	~Color() noexcept = default;
};

constexpr Color WHITE(1,1,1,1);
constexpr Color BLACK(0,0,0,1);
constexpr Color RED(1,0,0,1);
constexpr Color BLUE(0,0,1,1);
constexpr Color GREEN(0,1,0,1);
constexpr Color GRAY(.5,.5,.5,1);
}}
