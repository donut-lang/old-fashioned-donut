/*
 * Mapper25.h
 *
 *  Created on: 2011/09/18
 *      Author: psi
 */

#pragma once
#include "board/VRC4.h"

namespace nes {

class Mapper25 : public VRC4
{
public:
	Mapper25(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper25();

	/* for CPU */
	virtual void writeBankHigh(uint16_t addr, uint8_t val) override final;
	virtual void writeBankLow(uint16_t addr, uint8_t val) override final;
};

}
