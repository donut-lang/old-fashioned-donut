/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "MemoryComparator.hpp"
#include "../VirtualMachine.hpp"

namespace nes {

MemoryComparator::MemoryComparator(	VirtualMachine& vm)
:vm_(vm)
,candidates_(0)
{
	this->reset();
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
	this->reset();
}

void MemoryComparator::reset()
{
	this->candidates_=2048;
	this->last_.resize(2048);
	std::memset(this->entry_, static_cast<int>(-1), sizeof(this->entry_));
	std::memset(this->last_.data(), static_cast<int>(0), last_.size());
}

}
