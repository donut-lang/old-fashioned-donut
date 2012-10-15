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

#ifndef Chisa_GEOM_VECTOR_H__CPP_
#define Chisa_GEOM_VECTOR_H__CPP_

#include <cmath>
#include <string>
#include "../util/StringUtil.h"

#define SETUP(Klass)\
public:\
	Klass(const Klass& o):BaseVector<Klass>(o){};\
	Klass():BaseVector<Klass>(){};\
	explicit Klass(const float x, const float y):BaseVector<Klass>(x,y){};\
	virtual ~Klass(){};

#define ENABLE_CAST(Klass, From)\
	explicit Klass(const From& o):BaseVector<Klass>(o){};

#define ENABLE_UNARY_OP(Klass)\
	inline Klass operator-() const{\
		return this->BaseVector<Klass>::operator-();\
	}\
	inline Klass operator+() const{\
		return this->BaseVector<Klass>::operator+();\
	}

#define ENABLE_PM(Klass,OtherKlass,Result) \
	ENABLE_PLUS(Klass, OtherKlass, Result)\
	ENABLE_MINUS(Klass, OtherKlass, Result)

#define ENABLE_PLUS(Klass,OtherKlass,Result) \
public:\
	inline Result operator+(const OtherKlass& b) const{\
		return this->operator_add<Result, OtherKlass>(b);\
	};\

#define ENABLE_MINUS(Klass,OtherKlass,Result) \
public:\
	inline Result operator-(const OtherKlass& b) const{\
		return this->operator_minus<Result, OtherKlass>(b);\
	};

#define ENABLE_PM_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator+=(const OtherKlass& b) {\
			return operator_add_assign(*this, b);\
	};\
	inline Klass& operator-=(const OtherKlass& b) {\
			return operator_minus_assign(*this, b);\
	};
#define ENABLE_MD(Klass,OtherKlass,Result)\
	public:\
	inline Result operator*(const OtherKlass& b) const{\
		return this->operator_mult<Result, OtherKlass>(b);\
	};\
	inline Result operator/(const OtherKlass& b) const{\
		return this->operator_div<Result, OtherKlass>(b);\
	};
#define ENABLE_MD_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator*=(const OtherKlass& b) {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(const OtherKlass& b) {\
		return Klass::operator_div_assign(*this, b);\
	};
#define ENABLE_MD_FLOAT(Klass)\
	public:\
	inline Klass operator*(const float& b) const{\
		return this->operator_mult(b);\
	};\
	inline Klass operator/(const float& b) const{\
		return this->operator_div(b);\
	};\
	inline Klass& operator*=(const float& b) {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(const float& b) {\
		return Klass::operator_div_assign(*this, b);\
	};

namespace chisa {
namespace geom {
inline bool isUnspecified(const float width_or_height){
	return std::isnan(width_or_height);
};
inline bool isSpecified(const float width_or_height){
	return !std::isnan(width_or_height);
};
constexpr float Unspecified = NAN;
constexpr float VerySmall = 1e-1;

template <class Self>
class BaseVector {
	template <typename T> friend class BaseVector;
private:
	float x_;
	float y_;
protected:
	template <typename O>
	inline BaseVector(const BaseVector<O>& o):x_(o.x_),y_(o.y_){}
	inline BaseVector(const float x, const float y):x_(x),y_(y){}
	inline BaseVector():x_(NAN),y_(NAN){}
public:
	inline Self& operator=(const Self& other) {
		this->x_ = other.x_;
		this->y_ = other.y_;
		return *this;
	}
	inline bool operator==(const Self& other) const{
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline bool operator!=(const Self& other) const{
		return !(*this==other);
	}
	inline bool near(const Self& other, const float precision) const{
		return
				std::fabs(this->x_ - other.x_) < precision &&
				std::fabs(this->y_ - other.y_) < precision;
	}
	inline bool operator==(const Self& other){
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline bool operator!=(const Self& other){
		return !(*this==other);
	}
public:
	inline float x() const{return x_;};
	inline float y() const{return y_;};
	inline void x(const float x) { x_=x; };
	inline void y(const float y) { y_=y; };
	inline Self operator-() const{
		return Self(-x_,-y_);
	}
	inline Self operator+() const{
		return Self(+x_,+y_);
	}
protected:
	template <typename R, typename T>
	inline R operator_add(const T& other) const {
		return R(this->x_+other.x_, this->y_+other.y_);
	}
	template <typename R, typename T>
	inline R operator_minus(const T& other) const {
		return R(this->x_-other.x_, this->y_-other.y_);
	}
	template <typename T>
	static inline Self& operator_add_assign(Self& self, const T& other) {
		self.x_+=other.x_;
		self.y_+=other.y_;
		return self;
	}
	template <typename T>
	static inline Self& operator_minus_assign(Self& self, const T& other) {
		self.x_-=other.x_;
		self.y_-=other.y_;
		return self;
	}
	template <typename R, typename T>
	inline R operator_mult(const BaseVector<T>& other) const {
		return R(this->x_*other.x_, this->y_*other.y_);
	}
	inline Self operator_mult(const float& other) const {
		return Self(this->x_*other, this->y_*other);
	}
	template <typename T>
	static inline Self& operator_mult_assign(Self& self, const BaseVector<T>& other) {
		self.x_*=other.x_;
		self.y_*=other.y_;
		return self;
	}
	template <typename R>
	inline R operator_mult(float const ratio) const {
		return R(this->x_*ratio, this->y_*ratio);
	}
	static inline Self& operator_mult_assign(Self& self, const float& ratio) {
		self.x_*=ratio;
		self.y_*=ratio;
		return self;
	}
	template <typename R, typename T>
	inline R operator_div(const BaseVector<T>& other) const {
		return R(this->x_/other.x_, this->y_/other.y_);
	}
	inline Self operator_div(const float other) const {
		return Self(this->x_/other, this->y_/other);
	}
	template <typename T>
	static inline Self& operator_div_assign(Self& self, const BaseVector<T>& other) {
		self.x_/=other.x_;
		self.y_/=other.y_;
		return self;
	}
	static inline Self& operator_div_assign(Self& self, const float& ratio) {
		self.x_/=ratio;
		self.y_/=ratio;
		return self;
	}
	template <typename T>
	inline float dotProduct(const BaseVector<T>& other) const {
		return this->x_*other.x_+this->y_*other.y_;
	}
};

class ScaleVector : public BaseVector<ScaleVector> {
	SETUP(ScaleVector)
public:
	inline std::string toString() const{
		return util::format("(ScaleVector %f %f)", this->x(), this->y());
	}
};

class Vector : public BaseVector<Vector> {
	SETUP(Vector);
public:
	inline std::string toString() const{
		return util::format("(Vector %f %f)", this->x(), this->y());
	}
	ENABLE_UNARY_OP(Vector);
	ENABLE_PM(Vector , Vector , Vector );
	ENABLE_PM_ASSIGN(Vector, Vector);
	ENABLE_MD(Vector, ScaleVector, Vector);
	ENABLE_MD_ASSIGN(Vector, ScaleVector);
	ENABLE_MD_FLOAT(Vector);
	inline bool near(const Vector& other, const float precision) const{
		return
				std::fabs(this->x() - other.x()) < precision &&
				std::fabs(this->y() - other.y()) < precision;
	}
	inline float width() const{ return this->x(); };
	inline float height() const{ return this->y(); };
	inline void width(const float width) { this->x(width); };
	inline void height(const float height) { this->y(height); };
	inline bool empty() const {
		return
				std::fabs(this->x()) < geom::VerySmall ||
				std::fabs(this->y()) < geom::VerySmall;
	}
};

class Velocity : public BaseVector<Velocity> {
	SETUP(Velocity)
public:
	inline std::string toString() const{
		return util::format("(Velocity %f %f)", this->x(), this->y());
	}
	inline Velocity(const Vector& dist, const float time):BaseVector<Velocity>(dist/time){}
	ENABLE_UNARY_OP(Velocity);
	ENABLE_PM(Velocity, Velocity, Velocity);
	ENABLE_PM_ASSIGN(Velocity, Velocity);
	ENABLE_MD(Velocity, ScaleVector, Velocity);
	ENABLE_MD_ASSIGN(Velocity, ScaleVector);
};

template <typename T>
T min(const BaseVector<T>& a, const BaseVector<T>& b){
	return T(std::min(a.x(), b.x()), std::min(a.y(), b.y()));
}

template <typename T>
T max(const BaseVector<T>& a, const BaseVector<T>& b){
	return T(std::max(a.x(), b.x()), std::max(a.y(), b.y()));
}

}}

#undef ENABLE_PM
#undef ENABLE_PM_ASSGN
#undef ENABLE_MD
#undef ENABLE_MD_ASSGN
#undef FRIEND_OP
#undef ENABLE_CAST
#undef ENABLE_UNARY_OP
#undef SETUP

#include "Decl.h"

#endif /* INCLUDE_GUARD */
