/*
 * Mapper2.h
 *
 *  Created on: 2011/09/13
 *	  Author: psi
 */

#pragma once

#include "../VirtualMachine.h"
#include "../file/NesFile.h"

namespace nes {

class Mapper2 : public Cartridge
{
public:
	Mapper2(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper2();
	/* for PPU */
	virtual uint8_t readPatternTableHigh(uint16_t addr) override final;
	virtual void writePatternTableHigh(uint16_t addr, uint8_t val) override final;

	uint8_t readPatternTableLow(uint16_t addr) override final;
	void writePatternTableLow(uint16_t addr, uint8_t val) override final;

	/* for CPU */
	virtual uint8_t readBankHigh(uint16_t addr) override final;
	virtual void writeBankHigh(uint16_t addr, uint8_t val) override final;
	virtual uint8_t readBankLow(uint16_t addr) override final;
	virtual void writeBankLow(uint16_t addr, uint8_t val) override final;

	virtual uint8_t debuggerReadBankHigh(uint16_t addr) override final;
	virtual void debuggerWriteBankHigh(uint16_t addr, uint8_t val) override final;
	virtual uint8_t debuggerReadBankLow(uint16_t addr) override final;
	virtual void debuggerWriteBankLow(uint16_t addr, uint8_t val) override final;
private:
	uint8_t vram[8192];
	uint32_t lowBankAddrBase;
	const uint32_t highBankAddrBase;
};

}
