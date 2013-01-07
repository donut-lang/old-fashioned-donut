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
{
	// TODO Auto-generated constructor stub

}

std::vector<uint8_t> MemoryComparator::getNowMemory() const
{
	uint8_t (&ram)[Ram::WRAM_LENGTH] = vm_.ram().wram();
	return std::vector<uint8_t>(ram, ram+Ram::WRAM_LENGTH);
}

}
