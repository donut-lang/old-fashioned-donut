/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include <algorithm>
#include "Watcher.h"
#include "../fairy/DebuggerFairy.h"

namespace nes {

Watcher::Watcher(VirtualMachine& vm, DebuggerFairy& debuggerFairy)
:vm_(vm)
,debuggerFairy_(debuggerFairy)
,breakUniq_(0)
{
}
void Watcher::startStepRunning()
{
	addMemoryExecBreak(0,0xffff);
}

void Watcher::continueRunning()
{
	debuggerFairy_.onContinueBreak();
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

}
