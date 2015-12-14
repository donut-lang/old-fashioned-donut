/*
 * Mapper23.h
 *
 *  Created on: 2011/09/18
 *      Author: psi
 */

#pragma once
#include "board/VRC4.hpp"

namespace nes {

class Mapper23 : public VRC4
{
public:
	Mapper23(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper23();

	/* for CPU */
	virtual void writeBankHigh(uint16_t addr, uint8_t val) override final;
	virtual void writeBankLow(uint16_t addr, uint8_t val) override final;
};

}
