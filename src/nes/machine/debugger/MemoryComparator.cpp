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

#include "MemoryComparator.h"
#include "../VirtualMachine.h"

namespace nes {

MemoryComparator::MemoryComparator(	VirtualMachine& vm)
:vm_(vm)
,candidates_(2048)
{
	this->last_ = getNowMemory();
	std::memset(this->entry_, static_cast<int>(-1), sizeof(this->entry_)/sizeof(unsigned int));
}

std::vector<uint8_t> MemoryComparator::getNowMemory() const
{
	uint8_t (&ram)[Ram::WRAM_LENGTH] = vm_.ram().wram();
	return std::vector<uint8_t>(ram, ram+Ram::WRAM_LENGTH);
}

uint8_t MemoryComparator::now(uint16_t const& addr) const noexcept
{
	return this->vm_.ram().wram()[addr];
}

void MemoryComparator::start()
{
	this->candidates_=2048;
	this->last_ = getNowMemory();
	std::memset(this->entry_, static_cast<int>(-1), sizeof(this->entry_)/sizeof(unsigned int));
}

void MemoryComparator::reset()
{
	this->candidates_=2048;
	this->last_ = getNowMemory();
	std::memset(this->entry_, static_cast<int>(-1), sizeof(this->entry_)/sizeof(unsigned int));
}

}
