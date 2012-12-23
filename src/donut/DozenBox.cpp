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

#include "DozenBox.h"

namespace donut {

DozenBox::DozenBox(Logger& log)
:log_(log)
{
}

Handler<Object> DozenBox::execute( std::string const& src )
{
	Handler<Machine> const machine(this->mainDonut_->queryMachine());
	Handler<Object> const obj(machine->start(this->mainDonut_->parse(src)));
	if(machine->isInterrupted()){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops!! You cannot execute interrupted script in main donut!!");
	}
	return obj;
}

Handler<Heap> DozenBox::heap() const noexcept
{
	return this->mainDonut_->heap();
}

Handler<Object> DozenBox::startCombo(std::string const& comboname, std::string const& src)
{
	auto it = this->combos_.find(comboname);
	if( it != this->combos_.end() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Combo: \"%s\" is already started.", comboname.c_str());
	}
	Handler<Donut> donut(new Donut(this->log_));
	Handler<Source> parsed(donut->parse(src));
	return donut->queryMachine()->start(parsed);
}

Handler<Object> DozenBox::continueCombo(std::string const& comboname, Handler<Object> const& obj)
{
	auto it = this->combos_.find(comboname);
	if( it == this->combos_.end() ){
		throw DonutException(__FILE__, __LINE__, "[BUG] Oops. Combo: \"%s\" not found.", comboname.c_str());
	}
	Handler<Donut> combo = it->second;
	return combo->queryMachine()->resume( obj );
}

}
