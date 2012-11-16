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

#include "Disassembler.h"
#include "VirtualMachine.h"
#include "../../chisa/logging/Exception.h"

namespace nes {

Disassembler::Disassembler(VirtualMachine* vm)
:vm_(vm)
{

}
void Disassembler::decodeAt(uint16_t addr, Instruction* inst)
{
	inst->bin[0] = this->vm_->read(addr);
	const uint32_t flags = kInfoTable[inst->bin[0]];
	inst->op_ = static_cast<Operation>(flags & static_cast<uint32_t>(Operation::OperationMask));
	inst->addrMode_ = static_cast<AddrMode>(flags & static_cast<uint32_t>(AddrMode::AddrModeMask));
	inst->clock_ = (flags >> ClockShift) & 0xffff;

	switch(inst->addrMode_){
		case AddrMode::Immediate:
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->addr_ = addr+1;
			inst->val_ = inst->bin[1];
			break;
		case AddrMode::Zeropage:
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->addr_ = inst->bin[1];
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::ZeropageX:
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->addr_ = (inst->bin[1] + this->vm_->getProcessor()->getX()) & 0xff;
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::ZeropageY:
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->addr_ = (inst->bin[1] + this->vm_->getProcessor()->getY()) & 0xff;
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::Absolute:
			inst->binLength_ = 3;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->bin[2] = this->vm_->read(addr+2);
			inst->addr_ = inst->bin[1] | (inst->bin[2] << 8);
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::AbsoluteX:
			inst->binLength_ = 3;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->bin[2] = this->vm_->read(addr+2);
			inst->addr_ = (inst->bin[1] | (inst->bin[2] << 8)) + this->vm_->getProcessor()->getX();
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::AbsoluteY:
			inst->binLength_ = 3;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->bin[2] = this->vm_->read(addr+2);
			inst->addr_ = (inst->bin[1] | (inst->bin[2] << 8)) + this->vm_->getProcessor()->getY();
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::Indirect: {
			inst->binLength_ = 3;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->bin[2] = this->vm_->read(addr+2);
			const uint16_t base = inst->bin[1] | (inst->bin[2] << 8);
			inst->addr_ = this->vm_->read(base) | (this->vm_->read((base & 0xff00) | ((base + 1) & 0x00ff)) << 8); //bug of NES
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		}
		case AddrMode::IndirectX: {
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			const uint16_t base = (this->vm_->read(inst->bin[1]) + this->vm_->getProcessor()->getX()) & 0xff;
			inst->addr_ = this->vm_->read(base) | (this->vm_->read((base + 1) & 0xff) << 8);
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		}
		case AddrMode::IndirectY: {
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			const uint16_t base = inst->bin[1];
			inst->addr_ = ((this->vm_->read(base) | (this->vm_->read((base + 1) & 0xff) << 8)) + this->vm_->getProcessor()->getY());
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		}
		case AddrMode::Relative:
			inst->binLength_ = 2;
			inst->bin[1] = this->vm_->read(addr+1);
			inst->addr_ = inst->bin[1] >= 128 ? ( addr+2+inst->bin[1] - 256 )  : addr+2+inst->bin[1];
			inst->val_ = this->vm_->read(inst->addr_);
			break;
		case AddrMode::None:
			inst->binLength_=1;
			inst->addr_ = 0;
			inst->val_ = 0;
			break;
		default:
			throw chisa::logging::Exception(__FILE__, __LINE__, "[BUG] Oops. Unknown addr mode.");
	}
}

}
