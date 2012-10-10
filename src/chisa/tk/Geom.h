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

#ifndef Chisa_TK_GEOM_H__CPP_
#define Chisa_TK_GEOM_H__CPP_

#include <cmath>
#include <string>
#include "../util/StringUtil.h"

namespace chisa {
namespace tk {

namespace geom {
inline bool isUnspecified(const float width_or_height){
	return std::isnan(width_or_height);
};
inline bool isSpecified(const float width_or_height){
	return !std::isnan(width_or_height);
};
constexpr float Unspecified = NAN;
constexpr float VeryLarge = 1e10;
constexpr float VerySmall = 1e-1;
}

class Vector {
private:
	float x_;
	float y_;
public:
	inline Vector(const float x, const float y):x_(x),y_(y){}
	inline Vector(const Vector& o):x_(o.x_),y_(o.y_){}
	inline Vector():x_(NAN),y_(NAN){}
	inline Vector& operator=(const Vector& other) {
		this->x_ = other.x_;
		this->y_ = other.y_;
		return *this;
	}
	inline bool operator==(const Vector& other) const{
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline bool operator!=(const Vector& other) const{
		return !(*this==other);
	}
	inline bool near(const Vector& other, const float precision) const{
		return
				std::fabs(this->x_ - other.x_) < precision &&
				std::fabs(this->y_ - other.y_) < precision;
	}
	inline float x() const{return x_;};
	inline float y() const{return y_;};
	inline void x(const float x) { x_=x; };
	inline void y(const float y) { y_=y; };
};

class Distance : public Vector {
public:
	inline Distance(const float x, const float y):Vector(x,y){}
	inline Distance(const Distance& o):Vector(o){}
	inline Distance():Vector(){}
	inline std::string toString() const{
		return util::format("(Distance %f %f)", this->x(), this->y());
	}
	inline Distance operator-(const Distance& other) const{
		return Distance(this->x()-other.x(), this->y()-other.y());
	}
	inline Distance operator+(const Distance& d) const{
		return Distance(this->x()+d.x(), this->y()+d.y());
	}
	inline Distance& operator+=(const Distance& d){
		this->x(this->x()+d.x());
		this->y(this->y()+d.y());
		return *this;
	}
	inline Distance& operator-=(const Distance& d){
		this->x(this->x()-d.x());
		this->y(this->y()-d.y());
		return *this;
	}
};

class Point : public Vector {
public:
	inline Point(const float x, const float y):Vector(x,y){}
	inline Point(const Point& o):Vector(o){}
	inline Point():Vector(){}
	inline Point(Distance& dist):Vector(dist){};
	inline std::string toString() const{
		return util::format("(Point %f %f)", this->x(), this->y());
	}
	inline Distance operator-(const Point& other) const{
		return Distance(this->x()-other.x(), this->y()-other.y());
	}
	inline Point operator+(const Distance& d) const{
		return Point(this->x()+d.x(), this->y()+d.y());
	}
	inline Point& operator+=(const Distance& d){
		this->x(this->x()+d.x());
		this->y(this->y()+d.y());
		return *this;
	}
	inline Point& operator-=(const Distance& d){
		this->x(this->x()-d.x());
		this->y(this->y()-d.y());
		return *this;
	}
};

class Velocity : public Vector {
public:
	inline Velocity(const float x, const float y):Vector(x,y){}
	inline Velocity(const Velocity& o):Vector(o){}
	inline Velocity(const Distance& o, const float time):Vector(o.x()/time, o.y()/time){}
	inline Velocity():Vector(){}
	inline std::string toString() const{
		return util::format("(Velocity %f %f)", this->x(), this->y());
	}
};

class Box{
private:
	float width_;
	float height_;
public:
	inline Box(const float width, const float height):width_(width), height_(height){}
	inline Box(const Box& o):width_(o.width_), height_(o.height_){};
	inline Box():width_(NAN), height_(NAN){};
	inline Box& operator=(const Box& other){
		this->width_ = other.width_;
		this->height_ = other.height_;
		return *this;
	}
	inline bool operator==(const Box& other){
		return this->width_ == other.width_ && this->height_ == other.height_;
	}
	inline bool operator!=(const Box& other){
		return !(*this==other);
	}
	inline bool near(const Box& other, const float precision) const{
		return
				std::fabs(this->width_ - other.width_) < precision &&
				std::fabs(this->height_ - other.height_) < precision;
	}
	inline bool operator>(const Box& other) const{
		return this->width_ > other.width_ && this->height_ > other.height_;
	}
	inline bool operator>=(const Box& other) const{
		return this->width_ >= other.width_ && this->height_ >= other.height_;
	}
	inline bool operator<=(const Box& other) const{
		return this->width_ <= other.width_ && this->height_ <= other.height_;
	}
	inline bool operator<(const Box& other) const{
		return this->width_ < other.width_ && this->height_ < other.height_;
	}
	inline float width() const{ return width_; };
	inline float height() const{ return height_; };
	inline void width(const float width) { width_=width; };
	inline void height(const float height) { height_=height; };
	inline std::string toString() const{
		return util::format("(Box %f %f)", width_, height_);
	}
	inline bool empty() const {
		return
				std::fabs(this->width_) < geom::VerySmall &&
				std::fabs(this->height_) < geom::VerySmall;
	}
};

class Area
{
private:
	Point point_;
	Box box_;
public:
	inline Area(const float x, const float y, const float width, const float height):point_(x,y), box_(width, height){}
	inline Area(const Area& other):point_(other.point_), box_(other.box_){};
	inline Area(const Point& point, const Box& box):point_(point), box_(box){};
	inline Area& operator=(const Area& other){
		this->point_ = other.point_;
		this->box_ = other.box_;
		return *this;
	}
	inline bool near(const Area& other, const float precision) const{
		return this->box_.near(other.box_, precision) && point_.near(other.point_, precision);
	}
	inline Area():point_(), box_(){};
	inline Point& point() {return point_;};
	inline Box& box() {return box_;};
	inline const Point& point() const{return point_;};
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
		const Point startPoint(
				std::max(this->x(),other.x()),
				std::max(this->y(),other.y())
		);
		const Point endPoint(
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
	inline bool contain(const Point& pt) const{
		return x() <= pt.x() && y() <= pt.y() && pt.x() <= (x()+width()) && pt.y() <= (y()+height());
	}
};

}}

#endif /* INCLUDE_GUARD */
