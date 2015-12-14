/*
 * Mapper3.h
 *
 *  Created on: 2011/09/05
 *	  Author: psi
 */

#pragma once

#include "../VirtualMachine.hpp"
#include "../file/NesFile.hpp"

namespace nes {

class Mapper3 : public Cartridge
{
public:
	Mapper3(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper3();
	/* for PPU */
	virtual uint8_t readPatternTableHigh(uint16_t addr) override final;
	virtual uint8_t readPatternTableLow(uint16_t addr) override final;

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
	inline void writeReg(uint16_t addr, uint8_t val);
	const uint32_t prgHighBankAddrBase;
	uint32_t chrBankAddrBase;
};

}
