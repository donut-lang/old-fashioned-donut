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
#include <cmath>
#include <string>
#include "../util/StringUtil.h"

#define SETUP(Klass)\
public:\
	Klass(const Klass& o) noexcept = default;\
	Klass(Klass&& o) noexcept = default;\
	Klass& operator=(const Klass& o) noexcept = default;\
	Klass& operator=(Klass&& o) noexcept = default;\
	Klass() noexcept = default;\
	explicit Klass(const float x, const float y) noexcept:BaseVector<Klass>(x,y){};\
	~Klass() noexcept(true) = default;\
	void *operator new(std::size_t) = delete;\
	void operator delete(void* pv) = delete;

#define ENABLE_CAST(Klass, From)\
	explicit Klass(const From& o) noexcept:BaseVector<Klass>(o){};

#define ENABLE_UNARY_OP(Klass)\
	inline Klass operator-() const noexcept{\
		return this->BaseVector<Klass>::operator-();\
	}\
	inline Klass operator+() const noexcept{\
		return this->BaseVector<Klass>::operator+();\
	}

#define ENABLE_PM(Klass,OtherKlass,Result) \
	ENABLE_PLUS(Klass, OtherKlass, Result)\
	ENABLE_MINUS(Klass, OtherKlass, Result)

#define ENABLE_PLUS(Klass,OtherKlass,Result) \
public:\
	inline Result operator+(const OtherKlass& b) const noexcept{\
		return this->operator_add<Result, OtherKlass>(b);\
	};\

#define ENABLE_MINUS(Klass,OtherKlass,Result) \
public:\
	inline Result operator-(const OtherKlass& b) const noexcept{\
		return this->operator_minus<Result, OtherKlass>(b);\
	};

#define ENABLE_PM_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator+=(const OtherKlass& b) noexcept {\
			return operator_add_assign(*this, b);\
	};\
	inline Klass& operator-=(const OtherKlass& b) noexcept {\
			return operator_minus_assign(*this, b);\
	};
#define ENABLE_MD(Klass,OtherKlass,Result)\
	public:\
	inline Result operator*(const OtherKlass& b) const noexcept{\
		return this->operator_mult<Result, OtherKlass>(b);\
	};\
	inline Result operator/(const OtherKlass& b) const noexcept{\
		return this->operator_div<Result, OtherKlass>(b);\
	};
#define ENABLE_MD_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator*=(const OtherKlass& b) noexcept {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(const OtherKlass& b) noexcept {\
		return Klass::operator_div_assign(*this, b);\
	};
#define ENABLE_MD_FLOAT(Klass)\
	public:\
	inline Klass operator*(const float& b) const noexcept{\
		return this->operator_mult(b);\
	};\
	inline Klass operator/(const float& b) const noexcept{\
		return this->operator_div(b);\
	};\
	inline Klass& operator*=(const float& b) noexcept {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(const float& b) noexcept {\
		return Klass::operator_div_assign(*this, b);\
	};

namespace chisa {
namespace geom {
constexpr bool isUnspecified(const float width_or_height){
	return std::isnan(width_or_height);
};
constexpr bool isSpecified(const float width_or_height){
	return !std::isnan(width_or_height);
};
constexpr float max(const float a, const float b){
	return std::isnan(a) ? b : (a < b  ? b : a);
};
constexpr float min(const float a, const float b){
	return std::isnan(a) ? b : (a > b  ? b : a);
};
constexpr float Unspecified = NAN;
constexpr float VerySmall = 1e-1;

template <class Self>
class BaseVector {
	template <typename T> friend class BaseVector;
private:
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
	float x_;
	float y_;
protected:
	template <typename O>
	inline BaseVector(const BaseVector<O>& o) noexcept:x_(o.x_),y_(o.y_){}
	inline BaseVector(const BaseVector<Self>& s) = default;
	inline BaseVector(BaseVector<Self>&& s) = default;
	inline BaseVector<Self>& operator=(const BaseVector<Self>& s) = default;
	inline BaseVector<Self>& operator=(BaseVector<Self>&& s) = default;
	template <typename O>
	inline BaseVector(BaseVector<O>&& o) noexcept:x_(o.x_),y_(o.y_){}
	template <typename O>
	inline Self& operator=(BaseVector<O>&& o) noexcept{ x_ = o.x_; y_ = o.y_; return *this; }
	template <typename O>
	inline Self& operator=(const BaseVector<O>& o) noexcept{ x_ = o.x_; y_ = o.y_; return *this; }
	inline BaseVector(const float x, const float y) noexcept:x_(x),y_(y){}
	inline BaseVector() noexcept:x_(NAN),y_(NAN){}
	~BaseVector() noexcept = default;
public:
	inline Self& operator=(const Self& other) noexcept {
		this->x_ = other.x_;
		this->y_ = other.y_;
		return *this;
	}
	inline bool operator==(const Self& other) const noexcept{
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline bool operator!=(const Self& other) const noexcept{
		return !(*this==other);
	}
	inline bool near(const Self& other, const float precision) const noexcept{
		return
				std::fabs(this->x_ - other.x_) < precision &&
				std::fabs(this->y_ - other.y_) < precision;
	}
	inline bool operator==(const Self& other) noexcept{
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline bool operator!=(const Self& other) noexcept{
		return !(*this==other);
	}
public:
	inline float x() const noexcept{return x_;};
	inline float y() const noexcept{return y_;};
	inline void x(const float x) noexcept { x_=x; };
	inline void y(const float y) noexcept { y_=y; };
	inline Self flip() const noexcept { return Self(y_, x_); };
	inline Self operator-() const noexcept{
		return Self(-x_,-y_);
	}
	inline Self operator+() const noexcept{
		return Self(+x_,+y_);
	}
protected:
	template <typename R, typename T>
	inline R operator_add(const T& other) const noexcept {
		return R(this->x_+other.x_, this->y_+other.y_);
	}
	template <typename R, typename T>
	inline R operator_minus(const T& other) const noexcept {
		return R(this->x_-other.x_, this->y_-other.y_);
	}
	template <typename T>
	static inline Self& operator_add_assign(Self& self, const T& other) noexcept {
		self.x_+=other.x_;
		self.y_+=other.y_;
		return self;
	}
	template <typename T>
	static inline Self& operator_minus_assign(Self& self, const T& other) noexcept {
		self.x_-=other.x_;
		self.y_-=other.y_;
		return self;
	}
	template <typename R, typename T>
	inline R operator_mult(const BaseVector<T>& other) const noexcept {
		return R(this->x_*other.x_, this->y_*other.y_);
	}
	inline Self operator_mult(const float& other) const noexcept {
		return Self(this->x_*other, this->y_*other);
	}
	template <typename T>
	static inline Self& operator_mult_assign(Self& self, const BaseVector<T>& other) noexcept {
		self.x_*=other.x_;
		self.y_*=other.y_;
		return self;
	}
	template <typename R>
	inline R operator_mult(float const ratio) const noexcept {
		return R(this->x_*ratio, this->y_*ratio);
	}
	static inline Self& operator_mult_assign(Self& self, const float& ratio) noexcept {
		self.x_*=ratio;
		self.y_*=ratio;
		return self;
	}
	template <typename R, typename T>
	inline R operator_div(const BaseVector<T>& other) const noexcept {
		return R(this->x_/other.x_, this->y_/other.y_);
	}
	inline Self operator_div(const float other) const noexcept {
		return Self(this->x_/other, this->y_/other);
	}
	template <typename T>
	static inline Self& operator_div_assign(Self& self, const BaseVector<T>& other) noexcept {
		self.x_/=other.x_;
		self.y_/=other.y_;
		return self;
	}
	static inline Self& operator_div_assign(Self& self, const float& ratio) noexcept {
		self.x_/=ratio;
		self.y_/=ratio;
		return self;
	}
	template <typename T>
	inline float dotProduct(const BaseVector<T>& other) const noexcept {
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
	inline bool near(const Vector& other, const float precision) const noexcept{
		return
				std::fabs(this->x() - other.x()) < precision &&
				std::fabs(this->y() - other.y()) < precision;
	}
	inline float width() const noexcept{ return this->x(); };
	inline float height() const noexcept{ return this->y(); };
	inline void width(const float width) noexcept { this->x(width); };
	inline void height(const float height) noexcept { this->y(height); };
	inline bool empty() const noexcept {
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
T min(const BaseVector<T>& a, const BaseVector<T>& b) noexcept{
	return T(std::min(a.x(), b.x()), std::min(a.y(), b.y()));
}

template <typename T>
T max(const BaseVector<T>& a, const BaseVector<T>& b) noexcept{
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

