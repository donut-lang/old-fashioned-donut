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

#include "Closure.h"

namespace donut {

Closure::Closure(std::vector<std::string> const& arglist, std::vector<donut::Instruction> const& instlist)
:arglist_(arglist), instlist_(instlist)
{
}

Closure::Closure(XValue const& data)
{
	Handler<XObject> top(data.as<XObject>());
	for( XValue& s : *(top->get<XArray>("arglist")) ){
		const_cast<std::vector<std::string>& >(this->arglist_).push_back(s.as<std::string>());
	}
	for( XValue& s : *(top->get<XArray>("instlist")) ){
		const_cast<std::vector<donut::Instruction>& >(this->instlist_).push_back(s.as<donut::Instruction>());
	}
}

XValue Closure::save()
{
	Handler<XObject> top( new XObject );
	{
		Handler<XArray> list( new XArray );
		for(std::string const& val : this->arglist_){
			list->append(val);
		}
		top->set("arglist",list);
	}
	{
		Handler<XArray> list( new XArray );
		for(donut::Instruction const& val : this->instlist_){
			list->append(val);
		}
		top->set("instlist",list);
	}
	return top;
}

}
