/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "../Color.hpp"

#include <cinamo/XArchiver.h>
#include <cinamo/XMLAttrParser.h>

#include <cinamo/String.h>

using namespace chisa::gl;

namespace cinamo {
namespace xml {

template <>
void parseAttr<Color>(std::string const& name, Color& v, Color const& def, tinyxml2::XMLElement* elm)
{
	if( const char* attr = elm->Attribute(name.c_str()) ) {
		Color const c = Color::fromString(attr);
		if(!c.isInvalid()){
			v=c;
			return;
		}
	}
	v=def;
}

}

template <>
struct XSerializer<Color> {
	static XValue serialize(Color& val)
	{
		Handler<XObject> c(new XObject);
		c->set("red", val.red());
		c->set("green", val.green());
		c->set("blue", val.blue());
		c->set("alpha", val.alpha());
		return c;
	}
	static void deserialize(Color& val, XValue const& xval)
	{
		Handler<XObject> c(xval.as<XObject>());
		val.red( c->get<decltype(val.red())>("red") );
		val.green( c->get<decltype(val.green())>("green") );
		val.blue( c->get<decltype(val.blue())>("blue") );
		val.alpha( c->get<decltype(val.alpha())>("alpha") );
	}
};

}
