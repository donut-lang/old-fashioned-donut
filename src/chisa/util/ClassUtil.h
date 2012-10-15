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

#ifndef Chisa_UTIL_CLASS_UTILS_H__CPP_
#define Chisa_UTIL_CLASS_UTILS_H__CPP_

#define DISABLE_COPY_AND_ASSIGN(klass)\
private:\
	klass(const klass& other) = delete;\
	klass& operator=(const klass& other) = delete;\
	klass& operator=(klass&& other) = delete;

#define DEFINE_MEMBER_CONST(rscope, type, name)\
private:\
	type const name##_;\
rscope:\
	inline type name() const{return name##_;}\

#define DEFINE_MEMBER_REF(rscope, type, name)\
private:\
	type& name##_;\
rscope:\
	inline type& name() const{return name##_;}\

#define DEFINE_MEMBER(rscope, wscope, type, name)\
private:\
	type name##_;\
rscope:\
	inline type name() const{return name##_;}\
wscope:\
	inline void name(type const& val){name##_ = val;}

#endif /* INCLUDE_GUARD */