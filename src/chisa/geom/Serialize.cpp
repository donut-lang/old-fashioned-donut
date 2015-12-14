/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <cinamo/XMLAttrParser.h>
#include <cinamo/XArchiver.h>

#include "Vector.hpp"
#include "Area.hpp"

using namespace chisa::geom;

namespace cinamo {
namespace xml {

template <>
void parseAttr<Space>(std::string const& name, Space& v, Space const& def, tinyxml2::XMLElement* elm)
{
	float margin=0.0f;
	v=def;
	if(elm->QueryFloatAttribute(name.c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.margin(margin);
	}
	if(elm->QueryFloatAttribute((name+"-top").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.top(margin);
	}
	if(elm->QueryFloatAttribute((name+"-left").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.left(margin);
	}
	if(elm->QueryFloatAttribute((name+"-right").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.right(margin);
	}
	if(elm->QueryFloatAttribute((name+"-bottom").c_str(), &margin) == tinyxml2::XML_SUCCESS){
		v.bottom(margin);
	}
}

}

template <>
void XSerializer<Area>::deserialize(Area& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
	val.width(obj->get<decltype(val.width())>("width"));
	val.height(obj->get<decltype(val.height())>("height"));
}
template <>
XValue XSerializer<Area>::serialize(Area& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	obj->set("width", val.width());
	obj->set("height", val.height());
	return obj;
}

template <>
XValue XSerializer<Vector>::serialize(Vector& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	return obj;
}

template <>
void XSerializer<Vector>::deserialize(Vector& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
}

template <>
XValue XSerializer<IntVector>::serialize(IntVector& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("x", val.x());
	obj->set("y", val.y());
	return obj;
}

template <>
void XSerializer<IntVector>::deserialize(IntVector& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.x(obj->get<decltype(val.x())>("x"));
	val.y(obj->get<decltype(val.y())>("y"));
}

template <>
void XSerializer<Space>::deserialize(Space& val, XValue const& xval)
{
	Handler<XObject> obj(xval.as<XObject>());
	val.top(obj->get<decltype(val.top())>("top"));
	val.left(obj->get<decltype(val.left())>("left"));
	val.right(obj->get<decltype(val.right())>("right"));
	val.bottom(obj->get<decltype(val.bottom())>("bottom"));
}

template <>
XValue XSerializer<Space>::serialize(Space& val)
{
	Handler<XObject> obj(new XObject);
	obj->set("right", val.right());
	obj->set("left", val.left());
	obj->set("top", val.top());
	obj->set("bottom", val.bottom());
	return obj;
}

}
