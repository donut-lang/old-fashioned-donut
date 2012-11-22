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
,source_(new Source())
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

void Donut::bootstrap()
{
	this->heap_->bootstrap();
}

void Donut::onSeek( timestamp_t const& time )
{

}

Handler<Source> Donut::parse(const std::string& src, const std::string& filename, const int& lineno)
{
	return Parser::fromString(src, filename, lineno)->parseProgram();
}

}}
