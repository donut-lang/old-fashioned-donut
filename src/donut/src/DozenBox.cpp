/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#include <donut/DozenBox.hpp>

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
		DONUT_EXCEPTION(Exception, "[BUG] Oops!! You cannot execute interrupted script in main donut!!");
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
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Combo: \"%s\" is already scinamod.", comboname.c_str());
	}
	Handler<Donut> donut(new Donut(this->log_));
	Handler<Source> parsed(donut->parse(src));
	return donut->queryMachine()->start(parsed);
}

Handler<Object> DozenBox::continueCombo(std::string const& comboname, Handler<Object> const& obj)
{
	auto it = this->combos_.find(comboname);
	if( it == this->combos_.end() ){
		DONUT_EXCEPTION(Exception, "[BUG] Oops. Combo: \"%s\" not found.", comboname.c_str());
	}
	Handler<Donut> combo = it->second;
	return combo->queryMachine()->resume( obj );
}

}
