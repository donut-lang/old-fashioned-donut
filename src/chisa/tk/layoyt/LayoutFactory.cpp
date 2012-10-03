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
#include <algorithm>
#include "HorizontalLayout.h"
#include "VerticalLayout.h"

namespace chisa {
namespace tk {
namespace layout {

using std::shared_ptr;
using std::weak_ptr;
using std::auto_ptr;
using std::string;
using namespace tinyxml2;
using namespace logging;

LayoutFactory::LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename)
:log_(log)
,world_(world)
,doc_(new XMLDocument())
,doc_free_by_me_(true)
{
	const int code = doc_->LoadFile(filename.c_str());
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root_ = doc_->RootElement();
}

LayoutFactory::LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, XMLDocument* document, bool doc_free_by_me)
:log_(log)
,world_(world)
,doc_(document)
,doc_free_by_me_(doc_free_by_me)
{
	this->root_ = doc_->RootElement();
}

LayoutFactory::LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, const char* buffer, size_t lenb)
:log_(log)
,world_(world)
,doc_()
,doc_free_by_me_(true)
{
	const int code = doc_->Parse(buffer, lenb);
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root_ = doc_->RootElement();
}

LayoutFactory::~LayoutFactory()
{
	if(doc_free_by_me_){
		delete this->doc_;
	}
	this->doc_ = nullptr;
}

shared_ptr<Layout> LayoutFactory::parseTree(weak_ptr<Layout> root, weak_ptr<Layout> parent, XMLElement* top)
{
	string name = top->Name();
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if(name=="vertical"){
		return shared_ptr<Layout>(VerticalLayout::parseTree(*this, root, parent, top));
	} else if(name=="horizontal") {
		return shared_ptr<Layout>(HorizontalLayout::parseTree(*this, root, parent, top));
	} else if(name=="tab") {
	}else{
		throw logging::Exception(__FILE__,__LINE__, "Unknwon Layout: %s", name.c_str());
	}
}

shared_ptr<Layout> LayoutFactory::parseTree()
{
	return this->parseTree(weak_ptr<Layout>(), weak_ptr<Layout>(), this->root_);
}

}}}
