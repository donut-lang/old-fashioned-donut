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

#ifndef _CXX_Chisa_GEOM_AREA_H_
#define _CXX_Chisa_GEOM_AREA_H_
#include "Vector.h"

namespace chisa {
namespace geom {

class Area
{
private:
	Vector point_;
	Box box_;
public:
	inline Area(const float x, const float y, const float width, const float height):point_(x,y), box_(width, height){}
	inline Area(const Area& other):point_(other.point_), box_(other.box_){};
	inline Area(const Vector& point, const Box& box):point_(point), box_(box){};
	inline Area& operator=(const Area& other){
		this->point_ = other.point_;
		this->box_ = other.box_;
		return *this;
	}
	inline bool near(const Area& other, const float precision) const{
		return this->box_.near(other.box_, precision) && point_.near(other.point_, precision);
	}
	inline Area():point_(), box_(){};
	inline Vector& point() {return point_;};
	inline Box& box() {return box_;};
	inline const Vector& point() const{return point_;};
	inline const Box& box() const{return box_;};
	inline float x() const{ return point_.x(); };
	inline float y() const{ return point_.y(); };
	inline float width() const{ return box_.width(); };
	inline float height() const{ return box_.height(); };
	inline void x(const float x) { point_.x(x); };
	inline void y(const float y) { point_.y(y); };
	inline void width(const float width) { box_.width(width); };
	inline void height(const float height) { box_.height(height); };
	inline std::string toString() const{
		return util::format("(Area %f %f %f %f)", x(), y(), width(), height());
	}
	inline bool empty() const { return this->box_.empty(); }
	inline Area intersect(const Area& other) const
	{
		const float thisEndX = this->x()+this->width();
		const float thisEndY = this->y()+this->height();
		const float otherEndX = other.x()+other.width();
		const float otherEndY = other.y()+other.height();
		if(
				thisEndX <= other.x() ||
				otherEndX <= this->x() ||
				thisEndY <= other.y() ||
				otherEndY <= this->y()
		){
			return Area(0,0,0,0);
		}
		const Vector startPoint(
				std::max(this->x(),other.x()),
				std::max(this->y(),other.y())
		);
		const Vector endPoint(
				std::min(thisEndX,otherEndX),
				std::min(thisEndY,otherEndY)
		);
		return Area(
				startPoint.x(),
				startPoint.y(),
				endPoint.x()-startPoint.x(),
				endPoint.y()-startPoint.y()
		);
	}
	inline bool contain(const Vector& pt) const{
		return x() <= pt.x() && y() <= pt.y() && pt.x() <= (x()+width()) && pt.y() <= (y()+height());
	}
};

}}

#endif /* INCLUDE_GUARD */
