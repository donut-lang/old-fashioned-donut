/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <iostream>
#include <cinamo/Handler.h>
#include <donut/Patron.h>
#include <donut/Donut.h>
#include "parser/Parser.h"

namespace donut {

static Logger defaultLogger(std::cout, Logger::Level::WARN_);
static std::string const TAG("Donut");

Donut::Donut()
:log_(defaultLogger)
,clock_(new Clock(this))
,heap_(new Heap(log_, clock_))
,patron_()
{

}

Donut::Donut(Logger& log)
:log_(log)
,clock_(new Clock(this))
,heap_(new Heap(log_, clock_))
,patron_()
{

}

Donut::Donut(Logger& log, Handler<Patron> patron)
:log_(log)
,clock_(new Clock(this))
,heap_(new Heap(log_, clock_))
,patron_(patron)
{

}

Donut::Donut(Handler<Patron> patron)
:log_(defaultLogger)
,clock_(new Clock(this))
,heap_(new Heap(log_, clock_))
,patron_(patron)
{
}

Handler<Machine> Donut::queryMachine( std::string const& name )
{
	auto it = this->machines_.find(name);
	if(it != this->machines_.end()){
		return it->second;
	}else{
		Handler<Machine> m( new Machine(this->log(), this->clock_, this->heap_) );
		this->machines_.insert( name, m );
		return m;
	}
}

void Donut::sendMessage( std::string const& name, Handler<Object> const& obj )
{
	auto it = this->machines_.find(name);
	if(unlikely(it == this->machines_.end())){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Machine: \"%s\" not found.", name.c_str());
	}
	Handler<Machine> machine = it->second;
	machine->resume(obj);
}


Handler<Source> Donut::parse(std::string const& src, std::string const& filename, int const& lineno)
{
	if( this->log_.t() ){
		this->log_.t(TAG, "Parsing[%s:%d]:\n%s", filename.c_str(), lineno, src.c_str());
	}
	return heap_->registerSource ( Parser::fromString(src, filename, lineno)->parseProgram() );
}

/**********************************************************************************
 * save/restore function
 **********************************************************************************/
void Donut::bootstrap()
{
	// 1: 時計
	this->clock_->bootstrap();
	// 2: ヒープ
	if(this->patron_) {
		this->patron_->onRegisterProvider(heap_);
	}
	this->heap_->bootstrap();
	if(this->patron_) {
		this->patron_->onGlobalObjectInitialized(heap_);
	}
	// 3:マシン
	this->machines_.clear(); //すべて削除
	//
}
XValue Donut::save()
{
	Handler<XObject> top( new XObject );
	{ // 1: 時計
		top->set("clock", clock_->save());
	}
	{ // 2: ヒープ
		top->set("heap", heap_->save());
	}
	{ // 3: マシン
		Handler<XObject> machine( new XObject );
		for(std::pair<std::string const, Handler<Machine> > const& m : this->machines_) {
			machine->set(m.first, m.second->save());
		}
		top->set("machine", machine);
	}
	return top;
}
void Donut::load(XValue const& data)
{
	Handler<XObject> obj = data.as<XObject>();
	// 1: 時計
	{
		this->clock_->load(obj->get<XObject>("clock"));
	}
	{ // 2: ヒープ
		this->heap_->load(obj->get<XObject>("heap"));
	}
	{ // 3: マシン
		this->machines_.clear();
		for(std::pair<std::string, XValue>& val : *(obj->get<XObject>("machine"))){
			Handler<Machine> machine ( this->queryMachine(val.first) );
			machine->load( val.second );
		}
	}
}


/**********************************************************************************
 * time function
 **********************************************************************************/
unsigned int Donut::nowTime() const noexcept
{
	return this->clock_->now();
}
void Donut::seek( unsigned int const& time )
{
	this->clock_->seek( time );
}
void Donut::discardFuture()
{
	this->clock_->discardFuture();
}
void Donut::discardHistory()
{
	this->clock_->discardHistory();
}
unsigned int Donut::lastTime() const noexcept
{
	return this->clock_->lastTime();
}
unsigned int Donut::firstTime() const noexcept
{
	return this->clock_->firstTime();
}

bool Donut::canBack() const noexcept
{
	return this->clock_->canBack();
}
bool Donut::canAdvance() const noexcept
{
	return this->clock_->canAdvance();
}

/**********************************************************************************
 * from clock
 **********************************************************************************/

void Donut::onTickNotify()
{
	for( std::pair<std::string const,Handler<Machine> > const& m : this->machines_ ){
		m.second->onTickNotify();
	}
	this->heap_->onTickNotify();
}

void Donut::onBackNotify()
{
	for( std::pair<std::string const,Handler<Machine> > const& m : this->machines_ ){
		m.second->onBackNotify();
	}
	this->heap_->onBackNotify();
}
void Donut::onForwardNotify()
{
	for( std::pair<std::string const,Handler<Machine> > const& m : this->machines_ ){
		m.second->onForwardNotify();
	}
	this->heap_->onForwardNotify();
}

void Donut::onDiscardFutureNotify()
{
	for( std::pair<std::string const,Handler<Machine> > const& m : this->machines_ ){
		m.second->onDiscardFutureNotify();
	}
	this->heap_->onDiscardFutureNotify();
}
void Donut::onDiscardHistoryNotify()
{
	for( std::pair<std::string const,Handler<Machine> > const& m : this->machines_ ){
		m.second->onDiscardHistoryNotify();
	}
	this->heap_->onDiscardHistoryNotify();
}

}
