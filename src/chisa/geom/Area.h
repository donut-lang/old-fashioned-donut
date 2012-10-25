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
#include "Vector.h"

namespace chisa {
namespace geom {

class Area
{
private:
	Vector point_;
	Box box_;
private:
	inline constexpr Area intersectImpl(const Area& other, const Point& thisEnd, const Point& otherEnd) const noexcept{
		return
			thisEnd.x() <= other.x() || thisEnd.y() <= other.y() || otherEnd.x() <= this->x() || otherEnd.y() <= this->y() ?
			Area(0,0,0,0) :
			intersectImpl2(geom::max(this->point(), other.point()), geom::min(thisEnd, otherEnd));
	}
	inline constexpr Area intersectImpl2(const Point& startPoint, const Point& endPoint) const noexcept{
		return Area(startPoint, (endPoint-startPoint));
	}
public:
	constexpr Area(const float x, const float y, const float width, const float height) noexcept:point_(x,y), box_(width, height){}
	constexpr Area(const Area& other) noexcept = default;
	constexpr Area(Area&& other) noexcept = default;
	constexpr Area(const Vector& point, const Box& box) noexcept:point_(point), box_(box){};
	inline Area& operator=(const Area& other) noexcept = default;
	inline Area& operator=(Area&& other) noexcept = default;
	constexpr inline Area() noexcept:point_(), box_(){};
	inline ~Area() noexcept = default;
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
public:
	inline bool near(const Area& other, const float precision) const noexcept{
		return this->box_.near(other.box_, precision) && point_.near(other.point_, precision);
	}
	inline Vector& point() noexcept {return point_;};
	inline Box& box() noexcept {return box_;};
	inline constexpr const Vector& point() const noexcept{return point_;};
	inline constexpr const Box& box() const noexcept{return box_;};
	inline constexpr float x() const noexcept{ return point_.x(); };
	inline constexpr float y() const noexcept{ return point_.y(); };
	inline constexpr float width() const noexcept{ return box_.width(); };
	inline constexpr float height() const noexcept{ return box_.height(); };
	inline void x(const float x) noexcept { point_.x(x); };
	inline void y(const float y) noexcept { point_.y(y); };
	inline void width(const float width) noexcept { box_.width(width); };
	inline void height(const float height) noexcept { box_.height(height); };
	inline std::string toString() const{
		return util::format("(Area %f %f %f %f)", x(), y(), width(), height());
	}
	inline constexpr bool empty() const noexcept { return this->box_.empty(); }
	//	inline Area intersect(const Area& other) const noexcept
	//	{
	//		using namespace chisa::geom;
	//		const Point thisEnd = this->point()+this->box();
	//		const Point otherEnd = other.point()+other.box();
	//		if(
	//				thisEnd.x() <= other.point().x() ||
	//				thisEnd.y() <= other.point().y() ||
	//				otherEnd.x() <= this->point().x() ||
	//				otherEnd.y() <= this->point().y() ){
	//			return Area(0,0,0,0);
	//		}
	//		const Vector startPoint(max(this->point(), other.point()));
	//		const Vector endPoint(min(thisEnd, otherEnd));
	//		return Area(startPoint, (endPoint-startPoint));
	//	}
	inline constexpr Area intersect(const Area& other) const noexcept
	{
		return intersectImpl(other, this->point()+this->box(), other.point()+other.box());
	}
	inline constexpr bool contain(const Point& pt) const noexcept{
		return x() <= pt.x() && y() <= pt.y() && pt.x() <= (x()+width()) && pt.y() <= (y()+height());
	}
	inline constexpr Area flip() const noexcept {
		return Area(point_.flip(), box_.flip());
	}
};

}}
