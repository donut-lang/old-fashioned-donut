/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <algorithm>
#include "Watcher.hpp"
#include "../fairy/DebuggerFairy.hpp"

namespace nes {

Watcher::Watcher(VirtualMachine& vm, DebuggerFairy& debuggerFairy)
:vm_(vm)
,debuggerFairy_(debuggerFairy)
,breakUniq_(0)
{
}

void Watcher::stepRunning()
{
	debuggerFairy_.onStep();
}

void Watcher::resumeRunning()
{
	removeMemoryExecBreakAll();
	debuggerFairy_.onStep();
}
void Watcher::onBreak()
{
	debuggerFairy_.onBreak();
}

break_id_t Watcher::addMemoryReadBreak(uint16_t addr_begin, uint16_t addr_end)
{
	break_id_t const id = createBreakUniq();
	this->readBreaks_.push_back( MemoryRange(id, addr_begin, addr_end) );
	return id;
}

break_id_t Watcher::addMemoryWriteBreak(uint16_t addr_begin, uint16_t addr_end)
{
	break_id_t const id = createBreakUniq();
	this->writeBreaks_.push_back( MemoryRange(id, addr_begin, addr_end) );
	return id;
}

break_id_t Watcher::addMemoryExecBreak(uint16_t addr_begin, uint16_t addr_end)
{
	break_id_t const id = createBreakUniq();
	this->execBreaks_.push_back( MemoryRange(id, addr_begin, addr_end) );
	return id;
}

bool Watcher::removeMemoryReadBreak(break_id_t id)
{
	std::vector<MemoryRange>::iterator it = std::lower_bound(this->readBreaks_.begin(), this->readBreaks_.end(), id, MemoryRange::CompareByID());
	MemoryRange& range = *it;
	if(it == this->readBreaks_.end() || range.id != id){
		return false;
	}
	this->readBreaks_.erase(it);
	return true;
}

bool Watcher::removeMemoryWriteBreak(break_id_t id)
{
	std::vector<MemoryRange>::iterator const it = std::lower_bound(this->writeBreaks_.begin(), this->writeBreaks_.end(), id, MemoryRange::CompareByID());
	MemoryRange& range = *it;
	if(it == this->writeBreaks_.end() || range.id != id){
		return false;
	}
	this->writeBreaks_.erase(it);
	return true;
}

bool Watcher::removeMemoryExecBreak(break_id_t id)
{
	std::vector<MemoryRange>::iterator const it = std::lower_bound(this->execBreaks_.begin(), this->execBreaks_.end(), id, MemoryRange::CompareByID());
	MemoryRange& range = *it;
	if(it == this->execBreaks_.end() || range.id != id){
		return false;
	}
	this->execBreaks_.erase(it);
	return true;
}

std::vector<MemoryRange> Watcher::removeMemoryExecBreakAll()
{
	std::vector<MemoryRange> r;
	r.swap(execBreaks_);
	return r;
}

}
