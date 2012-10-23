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
public:
	inline Area(const float x, const float y, const float width, const float height) noexcept:point_(x,y), box_(width, height){}
	inline Area(const Area& other) noexcept:point_(other.point_), box_(other.box_){};
	inline Area(const Vector& point, const Box& box) noexcept:point_(point), box_(box){};
	inline Area& operator=(const Area& other) noexcept{
		this->point_ = other.point_;
		this->box_ = other.box_;
		return *this;
	}
	inline Area() noexcept:point_(), box_(){};
	inline ~Area() noexcept = default;
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
public:
	inline bool near(const Area& other, const float precision) const noexcept{
		return this->box_.near(other.box_, precision) && point_.near(other.point_, precision);
	}
	inline Vector& point() noexcept {return point_;};
	inline Box& box() noexcept {return box_;};
	inline const Vector& point() const noexcept{return point_;};
	inline const Box& box() const noexcept{return box_;};
	inline float x() const noexcept{ return point_.x(); };
	inline float y() const noexcept{ return point_.y(); };
	inline float width() const noexcept{ return box_.width(); };
	inline float height() const noexcept{ return box_.height(); };
	inline void x(const float x) noexcept { point_.x(x); };
	inline void y(const float y) noexcept { point_.y(y); };
	inline void width(const float width) noexcept { box_.width(width); };
	inline void height(const float height) noexcept { box_.height(height); };
	inline std::string toString() const{
		return util::format("(Area %f %f %f %f)", x(), y(), width(), height());
	}
	inline bool empty() const noexcept { return this->box_.empty(); }
	inline Area intersect(const Area& other) const noexcept
	{
		using namespace chisa::geom;
		const Point thisEnd = this->point()+this->box();
		const Point otherEnd = other.point()+other.box();
		if(
				thisEnd.x() <= other.point().x() ||
				thisEnd.y() <= other.point().y() ||
				otherEnd.x() <= this->point().x() ||
				otherEnd.y() <= this->point().y() ){
			return Area(0,0,0,0);
		}
		const Vector startPoint(max(this->point(), other.point()));
		const Vector endPoint(min(thisEnd, otherEnd));
		return Area(startPoint, (endPoint-startPoint));
	}
	inline bool contain(const Point& pt) const noexcept{
		return x() <= pt.x() && y() <= pt.y() && pt.x() <= (x()+width()) && pt.y() <= (y()+height());
	}
};

}}
