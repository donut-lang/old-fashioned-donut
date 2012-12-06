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

#include "Donut.h"
#include "parser/Parser.h"

namespace chisa {
namespace donut {

Donut::Donut(logging::Logger& log)
:log_(log)
,clock_(new Clock(this))
,heap_(new Heap(log_, clock_))
{

}

Handler<Machine> Donut::queryMachine( const std::string& name )
{
	auto it = this->machines_.find(name);
	if(it != this->machines_.end()){
		return it->second;
	}else{
		Handler<Machine> m( new Machine(this->log(), this->clock_, this->heap_) );
		this->machines_.insert( std::pair<std::string, Handler<Machine> >(name, m) );
		return m;
	}
}

void Donut::sendMessage( const std::string& name, const Handler<Object>& obj )
{
	auto it = this->machines_.find(name);
	if(it == this->machines_.end()){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Machine: \"%s\" not found.", name.c_str());
	}
	Handler<Machine> machine = it->second;
	machine->startContinue(obj);
}


Handler<Source> Donut::parse(const std::string& src, const std::string& filename, const int& lineno)
{
	return Parser::fromString(src, filename, lineno)->parseProgram();
}

/**********************************************************************************
 * save/restore function
 **********************************************************************************/
void Donut::bootstrap()
{
	//ヒープのブートを行う
	this->heap_->bootstrap();
	//マシンは何も起動しない -> TODO: すでにある状態を破棄できるようにする？
}
tinyxml2::XMLElement* Donut::save(tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* top = doc->NewElement("donut");
	{ // 1: 時計
		tinyxml2::XMLElement* clockElm = doc->NewElement("clock");
		clockElm->InsertEndChild( this->clock_->save(doc) );
		top->InsertEndChild( clockElm );
	}
	{ // 2: ヒープ
		tinyxml2::XMLElement* heapElm = doc->NewElement("heap");
		heapElm->InsertEndChild( this->heap_->save(doc) );
		top->InsertEndChild( heapElm );
	}
	{ // 3: マシン
		for(std::pair<std::string const, Handler<Machine> > const& m : this->machines_) {
			tinyxml2::XMLElement* mElm = doc->NewElement("machine");
			mElm->SetAttribute("name", m.first.c_str());
			mElm->InsertEndChild( m.second->save(doc) );
			top->InsertEndChild( mElm );
		}
	}

	return top;
}
void Donut::load(tinyxml2::XMLElement* data)
{
	// 1: 時計
	{
		tinyxml2::XMLElement* clockElm = data->FirstChildElement("clock");
		if( !clockElm ) {
			throw DonutException(__FILE__, __LINE__, "[BUG] Broken save file. \"clock\" element not found.");
		}
		this->clock_->load( clockElm->FirstChildElement() );
	}
	{ // 2: ヒープ
		tinyxml2::XMLElement* heapElm = data->FirstChildElement("heap");
		if( !heapElm ) {
			throw DonutException(__FILE__, __LINE__, "[BUG] Broken save file. \"heap\" element not found.");
		}
		this->heap_->load( heapElm->FirstChildElement() );
	}
	{ // 3: マシン
		this->machines_.clear();
		for(tinyxml2::XMLElement* mElm = data->FirstChildElement("machine"); mElm; mElm = mElm->NextSiblingElement("machine")){
			char const* name = mElm->Attribute("name");
			if( !name ) {
				throw DonutException(__FILE__, __LINE__, "[BUG] Broken save file. \"name\" attr of machine not found.");
			}
			Handler<Machine> machine ( this->queryMachine(name) );
			machine->load( mElm->FirstChildElement() );
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

void Donut::onSeekNotify()
{
	for( std::pair<std::string const,Handler<Machine>> const& m : this->machines_ ){
		m.second->onSeekNotify();
	}
	this->heap_->onSeekNotify();
}
void Donut::onDiscardFutureNotify()
{
	for( std::pair<std::string const,Handler<Machine>> const& m : this->machines_ ){
		m.second->onDiscardFutureNotify();
	}
	this->heap_->onDiscardFutureNotify();
}
void Donut::onDiscardHistoryNotify()
{
	for( std::pair<std::string const,Handler<Machine>> const& m : this->machines_ ){
		m.second->onDiscardHistoryNotify();
	}
	this->heap_->onDiscardHistoryNotify();
}

}}
