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

#include "Vector.h"
#include "Area.h"
#include "../util/XArchiver.h"

namespace chisa {
namespace util {

using namespace chisa::geom;
template <> inline void XArchiver::deserialize<Area>(Area& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
	val.width(obj->get<decltype(val.width())>("width"));
	val.height(obj->get<decltype(val.height())>("height"));
}
template <> inline XValue XArchiver::serialize<Area>(Area& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	obj->set("width", val.width());
	obj->set("height", val.height());
	return obj;
}

template <> inline XValue XArchiver::serialize<Vector>(Vector& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	return obj;
}

template <> inline void XArchiver::deserialize<Vector>(Vector& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
}

template <> inline XValue XArchiver::serialize<IntVector>(IntVector& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	return obj;
}

template <> inline void XArchiver::deserialize<IntVector>(IntVector& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
}

template <> inline void XArchiver::deserialize<Space>(Space& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.top(obj->get<decltype(val.top())>("top"));
	val.left(obj->get<decltype(val.left())>("left"));
	val.right(obj->get<decltype(val.right())>("right"));
	val.bottom(obj->get<decltype(val.bottom())>("bottom"));
}
template <> inline XValue XArchiver::serialize<Space>(Space& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("right", val.right());
	obj->set("left", val.left());
	obj->set("top", val.top());
	obj->set("bottom", val.bottom());
	return obj;
}

}}


