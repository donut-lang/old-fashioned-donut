/**
 * Donut
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

#include <donut/Donut.h>
#include <donut/Patron.h>
#include "parser/Parser.h"

namespace donut {

Donut::Donut(Logger& log, Handler<Patron> const& patron)
:log_(log)
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
	if(it == this->machines_.end()){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Machine: \"%s\" not found.", name.c_str());
	}
	Handler<Machine> machine = it->second;
	machine->resume(obj);
}


Handler<Source> Donut::parse(std::string const& src, std::string const& filename, int const& lineno)
{
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
	this->heap_->bootstrap();
	if(this->patron_) {
		this->patron_->onRegisterProvider(heap_);
		this->patron_->onRegisterInitialGlobalObject(heap_);
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
	this->clock_->discardFuture();
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
