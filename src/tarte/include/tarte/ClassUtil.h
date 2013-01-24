/**
 * Tarte
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
#include <cstddef>
#include "Common.h"
#define DISABLE_COPY_AND_ASSIGN(klass)\
	klass(klass const& other) = delete;\
	klass(klass&& other) = delete;\
	klass& operator=(klass const& other) = delete;\
	klass& operator=(klass&& other) = delete;

#define DEFAULT_COPY_AND_ASSIGN(klass)\
public:\
	klass(klass const& other) = default;\
	klass(klass&& other) = default;\
	klass& operator=(klass const& other) = default;\
	klass& operator=(klass&& other) = default;

#define STACK_OBJECT(Klass)\
	void* operator new(std::size_t) = delete;\
	void operator delete(void*) = delete;

//-----------------------------------------------------------------------------

#define DEFINE_MEMBER_CONST(rscope, type, name)\
private:\
	type const name##_;\
rscope:\
	inline type const& name() const noexcept{return name##_;}

#define DEFINE_MEMBER_REF(rscope, type, name)\
private:\
	type& name##_;\
rscope:\
	inline type& name() const noexcept{return name##_;}

#define DEFINE_MEMBER(rscope, wscope, type, name)\
private:\
	type name##_;\
rscope:\
	inline type const& name() const noexcept{return name##_;}\
wscope:\
	inline void name(type const& val){name##_ = val;}

#define DEFINE_CONTAINER(rscope, wscope, ctype, name)\
private:\
	ctype name##_;\
rscope:\
	inline ctype const& name() const noexcept{return name##_;}\
wscope:\
	inline ctype& name() noexcept{return name##_;}\
	inline void name(ctype const& val){name##_ = val;}

#define DEFINE_MEMBER_LITERAL(rscope, wscope, type, name)\
private:\
	type name##_;\
rscope:\
	inline constexpr type const& name() const noexcept{return name##_;}\
wscope:\
	inline void name(type const& val){name##_ = val;}
