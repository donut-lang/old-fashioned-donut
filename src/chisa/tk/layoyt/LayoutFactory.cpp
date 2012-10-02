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

#include "LayoutFactory.h"
#include "../Layout.h"
#include "../../logging/Exception.h"
#include <memory>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace layout {

using std::shared_ptr;
using std::weak_ptr;
using std::auto_ptr;
using std::string;
using namespace tinyxml2;
using namespace logging;

LayoutFactory::LayoutFactory(string& filename)
:doc()
{
	const int code = doc.LoadFile(filename.c_str());
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root = doc.RootElement();
}

LayoutFactory::LayoutFactory(string& filename, const char* buffer, size_t lenb)
:doc()
{
	const int code = doc.Parse(buffer, lenb);
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root = doc.RootElement();
}

LayoutFactory::~LayoutFactory()
{
	// TODO Auto-generated destructor stub
}

shared_ptr<Layout> LayoutFactory::parseInner(XMLElement* top)
{
	string name = top->Name();
	return shared_ptr<Layout>();
}

shared_ptr<Layout> LayoutFactory::create()
{
	return this->parseInner(this->root);
}

}}}
