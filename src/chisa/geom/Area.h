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
#include <tarte/ClassUtil.h>

namespace chisa {
namespace geom {

class Area
{
private:
	Vector point_;
	Box box_;
private:
	inline constexpr Area intersectImpl(Area const& other, Point const& thisEnd, Point const& otherEnd) const noexcept{
		return
			thisEnd.x() <= other.x() || thisEnd.y() <= other.y() || otherEnd.x() <= this->x() || otherEnd.y() <= this->y() ?
			Area(0,0,0,0) :
			intersectImpl2(geom::max(this->point(), other.point()), geom::min(thisEnd, otherEnd));
	}
	inline constexpr Area intersectImpl2(Point const& startPoint, Point const& endPoint) const noexcept{
		return Area(startPoint, (endPoint-startPoint));
	}
public:
	constexpr Area(const float x, const float y, const float width, const float height) noexcept:point_(x,y), box_(width, height){}
	constexpr Area(Area const& other) noexcept = default;
	constexpr Area(Area&& other) noexcept = default;
	constexpr Area(Vector const& point, Box const& box) noexcept:point_(point), box_(box){};
	inline Area& operator=(Area const& other) noexcept = default;
	inline Area& operator=(Area&& other) noexcept = default;
	constexpr inline Area() noexcept:point_(), box_(){};
	inline ~Area() noexcept = default;
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
public:
#ifdef near
#undef near
#endif
	inline bool near(Area const& other, const float precision) const noexcept{
		return this->box_.near(other.box_, precision) && point_.near(other.point_, precision);
	}
	inline Vector& point() noexcept {return point_;};
	inline Box& box() noexcept {return box_;};
	inline constexpr Vector const& point() const noexcept{return point_;};
	inline constexpr Box const& box() const noexcept{return box_;};
	inline constexpr float x() const noexcept{ return point_.x(); };
	inline constexpr float y() const noexcept{ return point_.y(); };
	inline constexpr float width() const noexcept{ return box_.width(); };
	inline constexpr float height() const noexcept{ return box_.height(); };
	inline void x(const float x) noexcept { point_.x(x); };
	inline void y(const float y) noexcept { point_.y(y); };
	inline void width(const float width) noexcept { box_.width(width); };
	inline void height(const float height) noexcept { box_.height(height); };
	inline std::string toString() const{
		return format("(Area %f %f %f %f)", x(), y(), width(), height());
	}
	inline constexpr bool empty() const noexcept { return this->box_.empty(); }
	inline constexpr Area operator*(ScaleVector const& scale){
		return Area(point()*scale, box()*scale);
	}
	inline constexpr Area operator/(ScaleVector const& scale){
		return Area(point()/scale, box()/scale);
	}
	//	inline Area intersect(Area const& other) const noexcept
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
	inline constexpr Area intersect(Area const& other) const noexcept
	{
		return intersectImpl(other, this->point()+this->box(), other.point()+other.box());
	}
	inline constexpr bool contain(Point const& pt) const noexcept{
		return x() <= pt.x() && y() <= pt.y() && pt.x() <= (x()+width()) && pt.y() <= (y()+height());
	}
	inline constexpr Area flip() const noexcept {
		return Area(point_.flip(), box_.flip());
	}
	inline constexpr float bottom() const noexcept {
		return this->y() + this->height();
	}
	inline constexpr float top() const noexcept {
		return this->y();
	}
	inline constexpr float right() const noexcept {
		return this->x() + this->width();
	}
	inline constexpr float left() const noexcept {
		return this->x();
	}
};

class Space {
	STACK_OBJECT(Space);
	DEFINE_MEMBER_LITERAL(public, public, float, top);
	DEFINE_MEMBER_LITERAL(public, public, float, bottom);
	DEFINE_MEMBER_LITERAL(public, public, float, left);
	DEFINE_MEMBER_LITERAL(public, public, float, right);
public:
	constexpr Space() noexcept :top_(0),bottom_(0),left_(0),right_(0){};
	constexpr Space(float const m) noexcept :top_(m),bottom_(m),left_(m),right_(m){};
	constexpr Space(float const top,float const bottom,float const left,float const right) noexcept :top_(top),bottom_(bottom),left_(left),right_(right){};
	constexpr Space(Space const& other) noexcept:
		top_(other.top_),bottom_(other.bottom_),left_(other.left_),right_(other.right_){};
	Space& operator=(Space const& other) noexcept{
		this->top_ = other.top_;
		this->bottom_ = other.bottom_;
		this->left_ = other.left_;
		this->right_ = other.right_;
		return *this;
	};
	Space& operator=(float const& other) noexcept{
		this->top_ = other;
		this->bottom_ = other;
		this->left_ = other;
		this->right_ = other;
		return *this;
	};
	void margin(float const& other) noexcept{
		(*this) = other;
	};
	inline constexpr geom::Area apply(geom::Area const& other)
	{
		return geom::Area(other.point() + this->offset(), other.box() - this->totalSpace());
	}
	inline constexpr Space operator+(Space const& other)
	{
		return Space(
				top_+other.top_,
				bottom_+other.bottom_,
				left_+other.left_,
				right_+other.right_
				);
	}
	inline constexpr float totalWidth() const noexcept { return this->left()+this->right(); };
	inline constexpr float totalHeight() const noexcept { return this->top()+this->bottom(); };
	inline constexpr geom::Distance totalSpace() const noexcept { return geom::Distance(totalWidth(), totalHeight()); };
	inline constexpr geom::Distance offset() const noexcept { return geom::Distance(this->left(), this->top()); };
	~Space() noexcept = default;
};

}}
