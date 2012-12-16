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

namespace chisa {
namespace util {

template <typename T> XArchiver& XArchiver::operator &(T& val) {
	if(decode_now_){
		XArchiver::deserialize( val, array_->get<XValue>(count_++) );
	}else{
		array_->append( XArchiver::serialize( val ) );
	}
	return *this;
}

template <typename T> inline auto XArchiver::serialize(T& val) -> typename std::enable_if< XArchiver::HasSerializer<T>::value, XValue >::type
{
	XArchiver a;
	val.serialize(a);
	return a.array_;
}
template <typename T> inline auto XArchiver::deserialize(T& val, XValue const& xval) -> typename std::enable_if< XArchiver::HasSerializer<T>::value, void >::type
{
	XArchiver arc(xval.as<XArray>());
	val.serialize(arc);
}

#define PRIM_VAR(TYPE)\
template <> inline void XArchiver::deserialize<typename _TypeAdapter<TYPE>::init_type>(typename _TypeAdapter<TYPE>::init_type& val, XValue const& xval)\
{\
	val = xval.as<TYPE>();\
}\
template <> inline XValue XArchiver::serialize<typename _TypeAdapter<TYPE>::init_type>(typename _TypeAdapter<TYPE>::init_type& val)\
{\
	return XValue(val);\
}
PRIM_VAR(XNull);
PRIM_VAR(XString);
PRIM_VAR(XUInt);
PRIM_VAR(XSInt);
PRIM_VAR(XFloat);
PRIM_VAR(XBool);
PRIM_VAR(XArray);
PRIM_VAR(XObject);

#undef PRIM_VAR

}}
