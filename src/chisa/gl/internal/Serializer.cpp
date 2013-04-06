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

#include "../Color.h"

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
