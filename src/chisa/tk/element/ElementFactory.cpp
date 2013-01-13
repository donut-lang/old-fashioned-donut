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

#include <tarte/Exception.h>
#include <tinyxml2.h>

#include "ElementFactory.h"
#include "../Element.h"

#include "SplitCombo.h"
#include "FrameCombo.h"
#include "Empty.h"
#include "ScrollCombo.h"
#include "WidgetElement.h"
#include "Button.h"
#include "ClockButton.h"
#include "Label.h"
#include "TabCombo.h"
#include "TextArea.h"

#include "../donut/element/SplitComboObject.h"
#include "../donut/element/TextAreaObject.h"

#include "../World.h"

namespace chisa {
namespace tk {

using namespace tinyxml2;
using namespace tarte;

const std::string ElementFactory::ElemName::World("world");
const std::string ElementFactory::ElemName::Vertical("vertical");
const std::string ElementFactory::ElemName::Horizontal("horizontal");
const std::string ElementFactory::ElemName::Frame("frame");
const std::string ElementFactory::ElemName::Empty("empty");
const std::string ElementFactory::ElemName::WidgetWrapper("widget");
const std::string ElementFactory::ElemName::Scroll("scroll");
const std::string ElementFactory::ElemName::Tab("tab");
const std::string ElementFactory::ElemName::Button("button");
const std::string ElementFactory::ElemName::ClockButton("clockbutton");
const std::string ElementFactory::ElemName::Label("label");
const std::string ElementFactory::ElemName::TextArea("textarea");

const std::string ElementFactory::AttrName::Id("id");

ElementFactory::ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename)
:log_(log)
,world_(world)
,filename_(filename)
,doc_(new tinyxml2::XMLDocument())
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

ElementFactory::ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename, tinyxml2::XMLDocument* document, bool doc_free_by_me)
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

ElementFactory::ElementFactory(Logger& log, HandlerW<World> world, std::string const& filename, const char* buffer, size_t lenb)
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
	this->registerLayout<SplitCombo>(ElemName::Horizontal);
	this->registerLayout<SplitCombo>(ElemName::Vertical);
	this->registerLayout<FrameCombo>(ElemName::Frame);
	this->registerLayout<Empty>(ElemName::Empty);
	this->registerLayout<ScrollCombo>(ElemName::Scroll);
	this->registerLayout<WidgetElement>(ElemName::WidgetWrapper);
	this->registerLayout<Button>(ElemName::Button);
	this->registerLayout<ClockButton>(ElemName::ClockButton);
	this->registerLayout<Label>(ElemName::Label);
	this->registerLayout<TabCombo>(ElemName::Tab);
	this->registerLayout<TextArea>(ElemName::TextArea);
}

ElementFactory::~ElementFactory()
{
	if(doc_free_by_me_){
		delete this->doc_;
	}
	this->doc_ = nullptr;
}

void ElementFactory::registerLayout(std::string const& xmlElementName, ElementFactory::ConstructorType constructor)
{
	if(!this->tagToElementConstructorMap_.insert(xmlElementName, constructor)){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. XML Element Name \"%s\" is already registered");
	}
}

void ElementFactory::registerProvider(std::string const& demangledElementName, Handler<ElementProvider> const& provider)
{
	if(!this->demangledElementNameToDonutProviderMap_.insert(demangledElementName, provider)){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. Provider for \"%s\" is already registered");
	}
	if( unlikely(!heap_) ) {
		TARTE_EXCEPTION(Exception, "[BUG] Oops. Heap is not specified.");
	}
	heap_->registerProvider(provider);
}

void ElementFactory::registerDonutProvider(Handler< ::donut::Heap> const& heap)
{
	HeapLock lock(*this, heap);
	Handler<World> world(this->world().lock());
	if( unlikely(!world) ){
		TARTE_EXCEPTION(Exception, "[BUG] Oops. World is already dead.");
	}
	this->registerProvider<SplitCombo>(Handler<ElementProvider>(new SplitComboProvider(heap, world)));
	this->registerProvider<TextArea>(Handler<ElementProvider>(new TextAreaProvider(heap, world)));
}

Handler<ElementProvider> ElementFactory::getProviderOf(Element* me)
{
	std::string const demangled(::tarte::demangle(me));
	VectorMap<std::string, Handler<ElementProvider> >::Iterator it =
			this->demangledElementNameToDonutProviderMap_.find(demangled);
	if(it == this->demangledElementNameToDonutProviderMap_.end()){
		return Handler<ElementProvider>();
	}
	VectorMap<std::string, Handler<ElementProvider> >::Pair const& p(*it);
	return p.second;
}

Handler<Element> ElementFactory::parseTree(HandlerW<Element> parent, XMLElement* top)
{
	const char* name = top->Name();
	auto it = this->tagToElementConstructorMap_.find(name);
	if(this->tagToElementConstructorMap_.end() == it){
		TARTE_EXCEPTION(Exception, "Unknwon Element: %s", name);
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
	TARTE_EXCEPTION(Exception, "ID \"%s\" not found in %s", elementId.c_str(), this->filename_.c_str());
}

}}
