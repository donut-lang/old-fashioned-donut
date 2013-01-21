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

#include "World.h"
#include "Element.h"
#include "Heaven.h"
#include "element/ElementFactory.h"
#include "element/WidgetElement.h"
#include "widget/WidgetFactory.h"
#include "Universe.h"
#include "Action.h"
#include "../gl/DrawableManager.h"
#include "../input/JoystickManager.h"
#include "Angel.h"
#include "donut/Patron.h"
#include "donut/WorldObject.h"

namespace chisa {
namespace tk {

const static std::string& TAG("World");

World::World(Logger& log, HandlerW<Universe> _universe, std::string const& worldname)
:log_(log)
,universe_(_universe)
,name_(worldname)
,taskHandler_(log)
,heaven_()
,doc_(nullptr)
,geist_(nullptr)
,elementFactory_(nullptr)
,widgetFactory_(nullptr)
,gestureMediator_(nullptr)
,patron_(nullptr)
{

}


World::~World() noexcept
{
	if(this->doc_){
		delete this->doc_;
		this->doc_=nullptr;
	}
	if(this->elementFactory_){
		delete this->elementFactory_;
		this->elementFactory_ = nullptr;
	}
	if(this->widgetFactory_){
		delete this->widgetFactory_;
		this->widgetFactory_ = nullptr;
	}
	if(this->gestureMediator_){
		delete this->gestureMediator_;
		this->gestureMediator_ = nullptr;
	}
}


void World::init()
{
	Handler<World> self(this->self());
	Handler<Universe> universe = this->universe_.lock();
	if( unlikely(!universe) ){
		TARTE_EXCEPTION(Exception, "[BUG] Universe is already dead.")
	}
	std::string const layoutfilename(universe->resolveWorldFilepath(this->name_, "layout.xml"));
	this->doc_ = new tinyxml2::XMLDocument();
	this->doc_->LoadFile(layoutfilename.c_str());

	this->elementFactory_ = new ElementFactory(this->log_, self, layoutfilename, this->doc_, false);
	this->widgetFactory_ = new WidgetFactory(this->log_, self);
	universe->hexe()->registerWidgets(*this->widgetFactory_);
	universe->hexe()->registerElements(*this->elementFactory_);

	initGeist(universe);

	this->heaven_ = Handler<Heaven>(new Heaven(self));
	this->gestureMediator_ = new ActionMediator(this->log_, self);

	{ //init-donut
		this->patron_ = Handler<Patron>(new Patron(universe, self));
		this->donut_ = Handler< ::donut::Donut>(new ::donut::Donut(this->log_, this->patron_));
		this->donut_->bootstrap();
		donut_->heap()->setGlobalObject("Geist", this->geist()->donutObject(donut_->heap()));
	}

	//init-element
	this->pushElement("main");

	//run-script
	for( tinyxml2::XMLElement* e = doc_->RootElement()->FirstChildElement("script"); e; e=e->NextSiblingElement("script") ) {
		char const* vm_name_ = e->Attribute("machine");
		std::string const vm_name ( vm_name_ ? vm_name_ : "" );
		if( char const* src = e->Attribute("src") ) {
			if( this->log().d() ) {
				this->log().d( TAG, "Running donut from: \"%s\"",src );
			}
			Handler<Machine> m = donut_->queryMachine(vm_name);
			m->start(donut_->parse(::tarte::file::readAsString(universe->resolveWorldFilepath(name_, src)), src, 1));
		}else if(char const* src = e->GetText()){
			if( this->log().d() ) {
				this->log().d( TAG, "Running embedded donut: \n\"%s\"",src );
			}
			Handler<Machine> m = donut_->queryMachine(vm_name);
			m->start(donut_->parse(src, "ScriptElement", 1));
		}else{
			this->log().w(TAG, "Oops. There is a script element but src is not specified.");
		}
	}
	donut_->discardHistory();
}

void World::initGeist(Handler<Universe> const& universe)
{
	if( const char* geistName = this->doc_->RootElement()->Attribute("geist", nullptr)){
		universe->hexe()->registerElements(*this->elementFactory_);
		this->geist(universe->invokeWorldGeist(this->self(), geistName));
	}else{
		log().w(TAG, "Geist not specified for: %s", this->name().c_str());
	}
}

void World::render(gl::Canvas& canvas)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->render(canvas, geom::ZERO, geom::Area(0,0,this->area().width(), this->area().height()));
		this->heaven_->render(canvas);
	}
}
void World::idle(const float delta_ms)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->idle(delta_ms);
		this->heaven_->idle(delta_ms);
	}
	this->taskHandler_.run(delta_ms);
}
void World::reshape(geom::Area const& area)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		elm->measure(area.box());
		elm->layout(geom::ZERO, area.box());
		this->heaven_->reshape(area);
	}
	this->area(area);
}

Handler< ::donut::Object> World::donutObject(Handler< ::donut::Heap> const& heap)
{
	if(!this->donutObject_.expired()){
		return this->donutObject_.lock();
	}
	Handler<WorldObject> obj( patron()->worldProvider()->newInstance(heap, self()) );
	this->donutObject_ = obj;
	return obj;
}

void World::pushElement(std::string const& elementId)
{
	this->elementStack_.push(realizeElement(elementId));
}

void World::popElement()
{
	this->elementStack_.pop();
	if(Handler<Element> elm = this->elementStack_.top()){
		this->reshape(this->area());
	}
}

void World::sendMessage( ::tarte::XValue const& xv, std::string const& machineName )
{
	if(this->log().d()){
		this->log().d(TAG, "Message received on \"%s\"", machineName.c_str());
	}
	this->sendTask([this, machineName, xv](){
		this->donut_->queryMachine(machineName)->resume(this->donut_->heap()->createObject(xv));
	});
}
void World::back()
{
	Handler< ::donut::Machine> vm = this->donut_->queryMachine();
	Handler< ::donut::Clock> clk = this->donut_->clock();
	if(!clk->canBack()){
		return;
	}
	clk->back();
}
void World::forward()
{
	Handler< ::donut::Machine> vm = this->donut_->queryMachine();
	Handler< ::donut::Clock> clk = this->donut_->clock();
	if(!clk->canAdvance()){
		return;
	}
	clk->forward();
}

Handler<Heaven> World::heaven()
{
	return heaven_;
}

Handler<Element> World::realizeElement( std::string const& templateId )
{
	return this->elementFactory_->parseTree(templateId);
}
Handler<Element> World::findElementByPoint(geom::Point const& screenPoint)
{
	Handler<Element> elm = this->heaven_->findElementByPoint(screenPoint);
	if(elm){
		return elm;
	}
	if(Handler<Element> elm = this->elementStack_.top()){
		return elm->findElementByPoint(screenPoint);
	}
	return Handler<Element>();
}
Handler<Element> World::findElementById(std::string const& id)
{
	if(Handler<Element> elm = this->elementStack_.top()){
		return elm->findElementById(id);
	}
	return Handler<Element>();
}

WidgetElement* World::getWidgetById(std::string const& name)
{
	auto it = this->widgetMap_.find(name);
	if(it == this->widgetMap_.end()){
		return nullptr;
	}
	return it->second;
}

bool World::replaceWidget(std::string const& widgetId, WidgetElement* const newHandler)
{
	std::unordered_map<std::string, WidgetElement*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		this->widgetMap_.erase(it);
	}
	this->widgetMap_.insert(std::pair<std::string, WidgetElement*>(widgetId, newHandler));
	return true;
}
bool World::deleteWidget(std::string const& widgetId, WidgetElement* const handler)
{
	std::unordered_map<std::string, WidgetElement*>::iterator it = this->widgetMap_.find(widgetId);
	if(it != widgetMap_.end()) {
		log_.w(TAG, "Oops. WidgetID: %s not found.", widgetId.c_str());
		return false;
	}
	if(it->second != handler) {
		log_.w(TAG, "Oops. WidgetID: %s query delete, but the handler is not correct.", widgetId.c_str());
		return false;
	}
	this->widgetMap_.erase(it);
	return true;
}

Widget* World::createWidget(std::string const& klass, tinyxml2::XMLElement* elem)
{
	return this->widgetFactory_->createWidget(klass, elem);
}

Handler<gl::DrawableManager> World::drawableManager() const
{
	if(Handler<Universe> uni = this->universe_.lock()){
		return uni->drawableManager();
	}else{
		return Handler<gl::DrawableManager>();
	}
}

void World::onTouchDown(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchDown(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchUp(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchUp(timeMs, pointerIndex, screenPoint);
	}
}

void World::onTouchMove(float const& timeMs, const unsigned int pointerIndex, geom::Point const& screenPoint)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTouchMove(timeMs, pointerIndex, screenPoint);
	}
}

void World::onScroll(float const& timeMs, const float delta)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onScroll(timeMs, delta);
	}
}

void World::onTextInput(float const& timeMs, std::string const& text)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTextInput(timeMs, text);
	}
}
void World::onTextEdit(float const& timeMs, std::string const& text, int start, int length)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onTextEdit(timeMs, text, start, length);
	}
}

void World::onKeyDown(float const& timeMs, bool isRepeat, SDL_Keysym const& sym)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onKeyDown(timeMs, isRepeat, sym);
	}
}
void World::onKeyUp(float const& timeMs, SDL_Keysym const& sym)
{
	if(this->gestureMediator_){
		this->gestureMediator_->onKeyUp(timeMs, sym);
	}
}

Handler<Quartet> World::quartet() const
{
	if(Handler<Universe> univ = this->universe_.lock()){
		return univ->quartet();
	}
	TARTE_EXCEPTION(Exception, "[BUG] Universe is already dead.");
}

Handler<JoystickManager> World::joystickManager() const
{
	if(Handler<Universe> univ = this->universe_.lock()){
		return univ->joystickManager();
	}
	TARTE_EXCEPTION(Exception, "[BUG] Universe is already dead.");
}


}}
