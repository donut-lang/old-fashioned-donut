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

class Point
{
private:
	float x_;
	float y_;
public:
	inline Point(const float x, const float y):x_(x),y_(y){}
	inline Point(const Point& o):x_(o.x_),y_(o.y_){}
	inline Point():x_(NAN),y_(NAN){}
	inline Point& operator=(const Point& other) {
		this->x_ = other.x_;
		this->y_ = other.y_;
		return *this;
	}
	inline float x() const{return x_;};
	inline float y() const{return y_;};
	inline std::string toString() const{
		return util::format("(Point %f %f)", x_, y_);
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
	inline float width() const{ return width_; };
	inline float height() const{ return height_; };
	inline static bool isUnspecified(const float width_or_height){ return std::isnan(width_or_height); };
	inline std::string toString() const{
		return util::format("(Box %f %f)", width_, height_);
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
	inline Area& operator=(const Area& other){
		this->point_ = other.point_;
		this->box_ = other.box_;
		return *this;
	}
	inline Area():point_(), box_(){};
	inline Point point() const{return point_;};
	inline Box box() const{return box_;};
	inline float x() const{ return point_.x(); };
	inline float y() const{ return point_.y(); };
	inline float width() const{ return box_.width(); };
	inline float height() const{ return box_.height(); };
	inline std::string toString() const{
		return util::format("(Area %f %f %f %f)", x(), y(), width(), height());
	}
};

}}

#endif /* INCLUDE_GUARD */
