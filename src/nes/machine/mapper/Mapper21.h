/*
 * Mapper21.h
 *
 *  Created on: 2011/09/18
 *      Author: psi
 */

#pragma once
#include "board/VRC4.h"

namespace nes {

class Mapper21 : public VRC4
{
public:
	Mapper21(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper21();
	/* for CPU */
	void writeBankHigh(uint16_t addr, uint8_t val);
	void writeBankLow(uint16_t addr, uint8_t val);
};

}
