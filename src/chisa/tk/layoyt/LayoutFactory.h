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

#ifndef Chisa_LAYOUTFACTORY_H__CPP_
#define Chisa_LAYOUTFACTORY_H__CPP_

#include <string>
#include <memory>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
class Layout;

namespace layout {
using std::string;
using std::shared_ptr;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

//TODO: 要素名とレイアウトの対応表

class LayoutFactory {
private:
	XMLDocument doc;
	XMLElement* root;
public:
	LayoutFactory(string& filename);
	LayoutFactory(string& filename, const char* buffer, std::size_t lenb);
	virtual ~LayoutFactory();
private:
	shared_ptr<Layout> parseInner(XMLElement* top);
public:
	shared_ptr<Layout> create();
};

}}}
#endif /* INCLUDE_GUARD */
