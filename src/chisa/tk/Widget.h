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

#ifndef Chisa_TK_WIDGET_H__CPP_
#define Chisa_TK_WIDGET_H__CPP_

#include <tr1/memory>
#include <unicode/unistr.h>
#include <cmath>
#include "../util/class_utils.h"

namespace chisa {
namespace tk {

class WidgetGroup;

class Point
{
private:
	const float x_;
	const float y_;
public:
	inline Point(const float x, const float y):x_(x),y_(y){}
	inline Point(const Point& o):x_(o.x_),y_(o.y_){}
	inline Point():x_(NAN),y_(NAN){}
	inline float x() const{return x_;};
	inline float y() const{return y_;};
};

class Box{
private:
	const float width_;
	const float height_;
public:
	inline Box(const float width, const float height):width_(width), height_(height){}
	inline Box(const Box& o):width_(o.width_), height_(o.height_){};
	inline Box():width_(NAN), height_(NAN){};
	inline float width() const{ return width_; };
	inline float height() const{ return height_; };
	inline static bool isUnspecified(const float width_or_height){ return std::isnan(width_or_height); };
};

class Area
{
private:
	const Point point_;
	const Box box_;
public:
	inline Area(const float x, const float y, const float width, const float height):point_(x,y), box_(width, height){}
	inline Area(const Area& other):point_(other.point_), box_(other.box_){};
	inline Area():point_(), box_(){};
	inline Point point() const{return point_;};
	inline float x() const{ return point_.x(); };
	inline float y() const{ return point_.y(); };
	inline float width() const{ return box_.width(); };
	inline float height() const{ return box_.height(); };
};

class Widget {
	DISABLE_COPY_AND_ASSIGN(Widget);
private:
	std::weak_ptr<WidgetGroup> root_;
	std::weak_ptr<WidgetGroup> parent_;
protected:
	std::weak_ptr<WidgetGroup> root() { return root_; };
	std::weak_ptr<WidgetGroup> parent() { return parent_; };
public:
	Widget(std::weak_ptr<WidgetGroup> root, std::weak_ptr<WidgetGroup> parent);
	virtual ~Widget();
public: /* レンダリング関連 */
	void invalidate();
	void render(const Area& area); /* OpenGLの座標の設定などを行なってしまう */
	virtual void renderImpl() = 0;
public:
	Point calcAbsolutePosition();
	Point calcRelativePositionFromParent();
	Box measure(const Box& box);
public:
};

}}
#endif /* INCLUDE_GUARD */
