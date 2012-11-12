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
#include "../util/ClassUtil.h"

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
#ifdef near
#undef near
#endif
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

class Margin {
	STACK_OBJECT(Margin);
	DEFINE_MEMBER_LITERAL(public, public, float, top);
	DEFINE_MEMBER_LITERAL(public, public, float, bottom);
	DEFINE_MEMBER_LITERAL(public, public, float, left);
	DEFINE_MEMBER_LITERAL(public, public, float, right);
public:
	constexpr Margin() noexcept :top_(0),bottom_(0),left_(0),right_(0){};
	constexpr Margin(float const m) noexcept :top_(m),bottom_(m),left_(m),right_(m){};
	constexpr Margin(float const top,float const bottom,float const left,float const right) noexcept :top_(top),bottom_(bottom),left_(left),right_(right){};
	constexpr Margin(const Margin& other) noexcept:
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
	inline constexpr geom::Area apply(const geom::Area other)
	{
		return geom::Area(other.point() + this->offset(), other.box() - this->totalSpace());
	}
	inline constexpr float totalWidth() const noexcept { return this->left()+this->right(); };
	inline constexpr float totalHeight() const noexcept { return this->top()+this->bottom(); };
	inline constexpr geom::Distance totalSpace() const noexcept { return geom::Distance(totalWidth(), totalHeight()); };
	inline constexpr geom::Distance offset() const noexcept { return geom::Distance(this->left(), this->top()); };
	~Margin() noexcept = default;
};

}}
