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
	constexpr Klass(Klass const& o) noexcept = default;\
	constexpr Klass(Klass&& o) noexcept = default;\
	Klass& operator=(Klass const& o) noexcept = default;\
	Klass& operator=(Klass&& o) noexcept = default;\
	Klass() noexcept = default;\
	constexpr explicit Klass(const float x, const float y) noexcept:BaseVector<Klass, Klass::ValType>(x,y){};\
	~Klass() noexcept(true) = default;\
	void *operator new(std::size_t) = delete;\
	void operator delete(void* pv) = delete;

#define ENABLE_CAST(Klass, From)\
	implicit constexpr Klass(From const& o) noexcept:BaseVector<Klass>(o){};

#define ENABLE_UNARY_OP(Klass)\
	inline constexpr Klass operator-() const noexcept{\
		return this->BaseVector<Klass, Klass::ValType>::operator-();\
	}\
	inline constexpr Klass operator+() const noexcept{\
		return this->BaseVector<Klass, Klass::ValType>::operator+();\
	}

#define ENABLE_PM(Klass,OtherKlass,Result) \
	ENABLE_PLUS(Klass, OtherKlass, Result)\
	ENABLE_MINUS(Klass, OtherKlass, Result)

#define ENABLE_PLUS(Klass,OtherKlass,Result) \
public:\
	inline constexpr Result operator+(OtherKlass const& b) const noexcept{\
		return this->operator_add<Result, OtherKlass>(b);\
	};\

#define ENABLE_MINUS(Klass,OtherKlass,Result) \
public:\
	inline constexpr Result operator-(OtherKlass const& b) const noexcept{\
		return this->operator_minus<Result, OtherKlass>(b);\
	};

#define ENABLE_PM_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator+=(OtherKlass const& b) noexcept {\
			return operator_add_assign(*this, b);\
	};\
	inline Klass& operator-=(OtherKlass const& b) noexcept {\
			return operator_minus_assign(*this, b);\
	};
#define ENABLE_MD(Klass,OtherKlass,Result)\
	public:\
	inline constexpr Result operator*(OtherKlass const& b) const noexcept{\
		return this->operator_mult<Result, OtherKlass>(b);\
	};\
	inline Result operator/(OtherKlass const& b) const noexcept{\
		return this->operator_div<Result, OtherKlass>(b);\
	};
#define ENABLE_MD_ASSIGN(Klass,OtherKlass)\
	public:\
	inline Klass& operator*=(OtherKlass const& b) noexcept {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(OtherKlass const& b) noexcept {\
		return Klass::operator_div_assign(*this, b);\
	};
#define ENABLE_MD_FLOAT(Klass)\
	public:\
	inline constexpr Klass operator*(float const& b) const noexcept{\
		return this->operator_mult(b);\
	};\
	inline constexpr Klass operator/(float const& b) const noexcept{\
		return this->operator_div(b);\
	};\
	inline Klass& operator*=(float const& b) noexcept {\
		return Klass::operator_mult_assign(*this, b);\
	};\
	inline Klass& operator/=(float const& b) noexcept {\
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

template <class Self, typename Val>
class BaseVector {
	template <typename T, typename V> friend class BaseVector;
private:
	void *operator new(std::size_t) = delete;
	void operator delete(void* pv) = delete;
	Val x_;
	Val y_;
protected:
	template <typename O>
	inline constexpr BaseVector(const BaseVector<O, Val>& o) noexcept:x_(o.x_),y_(o.y_){}
	inline constexpr BaseVector(const BaseVector<Self, Val>& s) = default;
	inline constexpr BaseVector(BaseVector<Self, Val>&& s) = default;
	inline BaseVector<Self, Val>& operator=(const BaseVector<Self, Val>& s) = default;
	inline BaseVector<Self, Val>& operator=(BaseVector<Self, Val>&& s) = default;
	template <typename O, typename V>
	inline constexpr BaseVector(BaseVector<O, V>&& o) noexcept:x_(o.x_),y_(o.y_){}
	template <typename O, typename V>
	inline Self& operator=(BaseVector<O, V>&& o) noexcept{ x_ = o.x_; y_ = o.y_; return *this; }
	template <typename O, typename V>
	inline Self& operator=(const BaseVector<O, V>& o) noexcept{ x_ = o.x_; y_ = o.y_; return *this; }
	constexpr BaseVector(const float x, const float y) noexcept:x_(x),y_(y){}
	constexpr BaseVector() noexcept:x_(NAN),y_(NAN){}
	~BaseVector() noexcept = default;
	typedef Val ValType;
public:
	inline Self& operator=(Self const& other) noexcept {
		this->x_ = other.x_;
		this->y_ = other.y_;
		return *this;
	}
	inline constexpr bool operator==(Self const& other) const noexcept{
		return this->x_ == other.x_ && this->y_ == other.y_;
	}
	inline constexpr bool operator!=(Self const& other) const noexcept{
		return !(*this==other);
	}
#ifdef near
#undef near
#endif
	inline constexpr bool near(Self const& other, const float precision) const noexcept{
		return
				std::fabs(this->x_ - other.x_) < precision &&
				std::fabs(this->y_ - other.y_) < precision;
	}
	inline bool constexpr isSpecified() const noexcept{
		return geom::isSpecified(this->x_) && geom::isSpecified(this->y_);
	}
public:
	inline constexpr Val x() const noexcept{return x_;};
	inline constexpr Val y() const noexcept{return y_;};
	inline void x(const Val x) noexcept { x_=x; };
	inline void y(const Val y) noexcept { y_=y; };
	inline constexpr Self flip() const noexcept { return Self(y_, x_); };
	inline constexpr Self operator-() const noexcept{
		return Self(-x_,-y_);
	}
	inline constexpr Self operator+() const noexcept{
		return Self(+x_,+y_);
	}
protected:
	template <typename R, typename T>
	inline constexpr R operator_add(const T& other) const noexcept {
		return R(this->x_+other.x_, this->y_+other.y_);
	}
	template <typename R, typename T>
	inline constexpr R operator_minus(const T& other) const noexcept {
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
	template <typename R, typename T, typename U>
	inline constexpr R operator_mult(const BaseVector<T, U>& other) const noexcept {
		return R(this->x_*other.x_, this->y_*other.y_);
	}
	inline constexpr Self operator_mult(float const& other) const noexcept {
		return Self(this->x_*other, this->y_*other);
	}
	template <typename T, typename U>
	static inline Self& operator_mult_assign(Self& self, const BaseVector<T, U>& other) noexcept {
		self.x_*=other.x_;
		self.y_*=other.y_;
		return self;
	}
	template <typename R>
	inline constexpr R operator_mult(float const ratio) const noexcept {
		return R(this->x_*ratio, this->y_*ratio);
	}
	static inline Self& operator_mult_assign(Self& self, float const& ratio) noexcept {
		self.x_*=ratio;
		self.y_*=ratio;
		return self;
	}
	template <typename R, typename T, typename U>
	inline constexpr R operator_div(const BaseVector<T, U>& other) const noexcept {
		return R(this->x_/other.x_, this->y_/other.y_);
	}
	inline constexpr Self operator_div(const float other) const noexcept {
		return Self(this->x_/other, this->y_/other);
	}
	template <typename T, typename U>
	static inline Self& operator_div_assign(Self& self, const BaseVector<T, U>& other) noexcept {
		self.x_/=other.x_;
		self.y_/=other.y_;
		return self;
	}
	static inline Self& operator_div_assign(Self& self, float const& ratio) noexcept {
		self.x_/=ratio;
		self.y_/=ratio;
		return self;
	}
	template <typename T, typename U>
	inline constexpr float dotProduct(const BaseVector<T, U>& other) const noexcept {
		return this->x_*other.x_+this->y_*other.y_;
	}
};
class ScaleVector : public BaseVector<ScaleVector, float> {
	SETUP(ScaleVector)
public:
	inline std::string toString() const{
		return util::format("(ScaleVector %f %f)", this->x(), this->y());
	}
};

class IntVector;

class Vector : public BaseVector<Vector, float> {
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
	inline constexpr bool near(Vector const& other, const ValType precision) const noexcept{
		return
				std::fabs(this->x() - other.x()) < precision &&
				std::fabs(this->y() - other.y()) < precision;
	}
	inline constexpr ValType width() const noexcept{ return this->x(); };
	inline constexpr ValType height() const noexcept{ return this->y(); };
	inline void width(const ValType width) noexcept { this->x(width); };
	inline void height(const ValType height) noexcept { this->y(height); };
	inline constexpr bool empty() const noexcept {
		return
				std::fabs(this->x()) < geom::VerySmall ||
				std::fabs(this->y()) < geom::VerySmall;
	}
};
constexpr Vector ZERO = Vector(0,0);

class IntVector : public BaseVector<IntVector, int> {
	SETUP(IntVector);
public:
	inline std::string toString() const{
		return util::format("(IntVector %f %f)", this->x(), this->y());
	}
	ENABLE_UNARY_OP(IntVector);
	ENABLE_PM(IntVector , IntVector , IntVector );
	ENABLE_PM(IntVector , Vector , Vector );
	ENABLE_PM_ASSIGN(IntVector, IntVector);
	inline constexpr ValType width() const noexcept{ return this->x(); };
	inline constexpr ValType height() const noexcept{ return this->y(); };
	inline constexpr operator Vector() {
		return Vector(x(),y());
	}
	inline void width(const ValType width) noexcept { this->x(width); };
	inline void height(const ValType height) noexcept { this->y(height); };
	inline constexpr bool empty() const noexcept {
		return this->x() <= 0 || this->y() <= 0;
	}
};

class Velocity : public BaseVector<Velocity, float> {
	SETUP(Velocity)
public:
	inline std::string toString() const{
		return util::format("(Velocity %f %f)", this->x(), this->y());
	}
	inline constexpr Velocity(Vector const& dist, const float time):BaseVector<Velocity, float>(dist/time){}
	ENABLE_UNARY_OP(Velocity);
	ENABLE_PM(Velocity, Velocity, Velocity);
	ENABLE_PM_ASSIGN(Velocity, Velocity);
	ENABLE_MD(Velocity, ScaleVector, Velocity);
	ENABLE_MD_ASSIGN(Velocity, ScaleVector);
};

template <typename T, typename U>
constexpr T min(const BaseVector<T, U>& a, const BaseVector<T, U>& b) noexcept{
	return T(std::min(a.x(), b.x()), std::min(a.y(), b.y()));
}

template <typename T, typename U>
constexpr T max(const BaseVector<T, U>& a, const BaseVector<T, U>& b) noexcept{
	return T(std::max(a.x(), b.x()), std::max(a.y(), b.y()));
}

}}

#undef SETUP
#undef ENABLE_CAST
#undef ENABLE_UNARY_OP
#undef ENABLE_PM
#undef ENABLE_PLUS
#undef ENABLE_MINUS
#undef ENABLE_PM_ASSIGN
#undef ENABLE_MD
#undef ENABLE_MD_ASSIGN
#undef ENABLE_MD_FLOAT

#include "Decl.h"

