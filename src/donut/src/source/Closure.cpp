/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/source/Closure.hpp>

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
