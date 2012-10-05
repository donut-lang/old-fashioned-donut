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
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "EmptyLayout.h"

namespace chisa {
namespace tk {
namespace layout {

using std::shared_ptr;
using std::weak_ptr;
using std::auto_ptr;
using std::string;
using namespace tinyxml2;
using namespace logging;

const std::string LayoutFactory::ElemName::World("world");
const std::string LayoutFactory::ElemName::Vertical("vertical");
const std::string LayoutFactory::ElemName::Horizontal("horizontal");
const std::string LayoutFactory::ElemName::Empty("empty");
const std::string LayoutFactory::ElemName::WidgetWrapper("widget");
const std::string LayoutFactory::ElemName::Tab("tab");

const std::string LayoutFactory::AttrName::Id("id");

LayoutFactory::LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename)
:log_(log)
,world_(world)
,filename_(filename)
,doc_(new XMLDocument())
,doc_free_by_me_(true)
{
	const int code = doc_->LoadFile(filename.c_str());
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root_ = doc_->RootElement();
	if(!this->root_ && ElemName::World == this->root_->Name()){
		throw Exception(__FILE__, __LINE__, "%s was parsed, but \"world\" element not found.", filename.c_str());
	}
}

LayoutFactory::LayoutFactory(logging::Logger& log, weak_ptr<World> world, const string& filename, XMLDocument* document, bool doc_free_by_me)
:log_(log)
,world_(world)
,filename_(filename)
,doc_(document)
,doc_free_by_me_(doc_free_by_me)
{
	this->root_ = doc_->RootElement();
	if(!this->root_ && ElemName::World == this->root_->Name()){
		throw Exception(__FILE__, __LINE__, "%s was parsed, but \"world\" element not found.", filename.c_str());
	}
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
	shared_ptr<Layout> layout;
	const char* name = top->Name();
	if(false){
	} else if(ElemName::Vertical == name){
		std::shared_ptr<Layout>(new VerticalLayout(this->log(), this->world(), root, parent)).swap(layout);
	} else if(ElemName::Horizontal == name){
		std::shared_ptr<Layout>(new HorizontalLayout(this->log(), this->world(), root, parent)).swap(layout);
	} else if(ElemName::Tab == name) {
//		std::shared_ptr<Layout>(new TabLayout(this->log(), this->world(), root, parent)).swap(layout);
	} else if(ElemName::Empty == name) {
		std::shared_ptr<Layout>(new EmptyLayout(this->log(), this->world(), root, parent)).swap(layout);
	}else{
		throw logging::Exception(__FILE__,__LINE__, "Unknwon Layout: %s", name);
	}
	layout->loadXML(this, top);
	return layout;
}

shared_ptr<Layout> LayoutFactory::parseTree(const string& layoutname)
{
	for(XMLElement* elem = this->root_->FirstChildElement(); elem; elem = elem->NextSiblingElement()){
		if(layoutname == elem->Attribute(AttrName::Id.c_str(), nullptr)){
			return this->parseTree(weak_ptr<Layout>(), weak_ptr<Layout>(), elem);
		}
	}
	throw new logging::Exception(__FILE__, __LINE__, "Failed to load layout: %s in %s", layoutname.c_str(), this->filename_.c_str());
}

}}}
