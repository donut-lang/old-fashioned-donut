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
#include "../../util/XMLUtil.h"
#include "../../util/StringUtil.h"

namespace chisa {
namespace util {
namespace xml {

using namespace chisa::gl;


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
}}}


