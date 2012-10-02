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

#include "../Layout.h"
#include "../../logging/Exception.h"
#include <memory>
#include <stdio.h>
#include <tinyxml2.h>

namespace chisa {
namespace tk {
namespace layout {

using std::shared_ptr;
using std::weak_ptr;
using std::auto_ptr;
using std::string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

shared_ptr<Layout> loadLayoutFromMemory(const char* buffer, const size_t len)
{
	XMLDocument doc;
	doc.Parse(buffer, len);
	XMLElement* root = doc.RootElement();
}

shared_ptr<Layout> loadLayoutFromFile(string& filename)
{
	FILE* f = fopen(filename.c_str(), "rb");
	fseeko64(f, 0LLU, SEEK_SET);
	const off64_t beg = ftello64(f);
	fseeko64(f, 0LLU, SEEK_END);
	const off64_t size = ftello64(f)-beg;
	auto_ptr<char> dat(new char[size]);
	if ( fread(dat.get(), size, 1, f) != 0 ){
		throw logging::Exception(__FILE__, __LINE__, "Failed to read file: %s", filename.c_str());
	}
	return loadLayoutFromMemory(dat.get(), size);
}

}}}

