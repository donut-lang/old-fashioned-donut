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

#include "ElementFactory.h"
#include "../Element.h"
#include "../../logging/Exception.h"

#include "SplitCombo.h"
#include "FrameCombo.h"
#include "Empty.h"
#include "ScrollCombo.h"
#include "WidgetElement.h"
#include "Button.h"
#include "TabCombo.h"

#include <tinyxml2.h>

namespace chisa {
namespace tk {

using namespace tinyxml2;
using namespace logging;

const std::string ElementFactory::ElemName::World("world");
const std::string ElementFactory::ElemName::Vertical("vertical");
const std::string ElementFactory::ElemName::Horizontal("horizontal");
const std::string ElementFactory::ElemName::Frame("frame");
const std::string ElementFactory::ElemName::Empty("empty");
const std::string ElementFactory::ElemName::WidgetWrapper("widget");
const std::string ElementFactory::ElemName::Scroll("scroll");
const std::string ElementFactory::ElemName::Tab("tab");
const std::string ElementFactory::ElemName::Button("button");

const std::string ElementFactory::AttrName::Id("id");

ElementFactory::ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename)
:log_(log)
,world_(world)
,filename_(filename)
,doc_(new XMLDocument())
,doc_free_by_me_(true)
{
	init();
	const int code = doc_->LoadFile(filename.c_str());
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root_ = doc_->RootElement();
	if(!this->root_ && ElemName::World == this->root_->Name()){
		throw Exception(__FILE__, __LINE__, "%s was parsed, but \"world\" element not found.", filename.c_str());
	}
}

ElementFactory::ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename, XMLDocument* document, bool doc_free_by_me)
:log_(log)
,world_(world)
,filename_(filename)
,doc_(document)
,doc_free_by_me_(doc_free_by_me)
{
	init();
	this->root_ = doc_->RootElement();
	if(!this->root_ || ElemName::World != this->root_->Name()){
		throw Exception(__FILE__, __LINE__, "%s was parsed, but \"world\" element not found.", filename.c_str());
	}
}

ElementFactory::ElementFactory(logging::Logger& log, HandlerW<World> world, std::string const& filename, const char* buffer, size_t lenb)
:log_(log)
,world_(world)
,doc_()
,doc_free_by_me_(true)
{
	init();
	const int code = doc_->Parse(buffer, lenb);
	if(code != XML_NO_ERROR){
		throw Exception(__FILE__, __LINE__, "Failed to read xml (%d): %s", code, filename.c_str());
	}
	this->root_ = doc_->RootElement();
}

void ElementFactory::init()
{
	this->registerElement<SplitCombo>(ElemName::Horizontal);
	this->registerElement<SplitCombo>(ElemName::Vertical);
	this->registerElement<FrameCombo>(ElemName::Frame);
	this->registerElement<Empty>(ElemName::Empty);
	this->registerElement<ScrollCombo>(ElemName::Scroll);
	this->registerElement<WidgetElement>(ElemName::WidgetWrapper);
	this->registerElement<Button>(ElemName::Button);
	this->registerElement<TabCombo>(ElemName::Tab);
}

ElementFactory::~ElementFactory()
{
	if(doc_free_by_me_){
		delete this->doc_;
	}
	this->doc_ = nullptr;
}

void ElementFactory::registerElement(std::string const& elementName, ElementFactory::ConstructorType constructor)
{
	this->elementMap_.update(elementName, constructor);
}

Handler<Element> ElementFactory::parseTree(HandlerW<Element> parent, XMLElement* top)
{
	const char* name = top->Name();
	auto it = this->elementMap_.find(name);
	if(this->elementMap_.end() == it){
		throw logging::Exception(__FILE__,__LINE__, "Unknwon Element: %s", name);
	}
	Handler<Element> elm(it->second(this->log(), this->world(), parent));
	elm->loadXml(this, top);
	return elm;
}

Handler<Element> ElementFactory::parseTree(std::string const& elementId)
{
	for(XMLElement* elem = this->root_->FirstChildElement(); elem; elem = elem->NextSiblingElement()){
		const char* id = elem->Attribute(AttrName::Id.c_str(), nullptr);
		if(id && elementId == id){
			return this->parseTree(HandlerW<Element>(), elem);
		}
	}
	throw logging::Exception(__FILE__, __LINE__, "Element ID \"%s\" not found in %s", elementId.c_str(), this->filename_.c_str());
}

}}
