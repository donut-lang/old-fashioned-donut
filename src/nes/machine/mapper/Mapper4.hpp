/*
 * Mapper4.h
 *
 *  Created on: 2011/09/20
 *	  Author: psi
 */

#pragma once
#include "../VirtualMachine.hpp"

namespace nes {

class Mapper4 : public Cartridge
{
public:
	Mapper4(VirtualMachine& vm, const NesFile* nesFile);
	virtual ~Mapper4();
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
	enum{
		CHR_BANK_SIZE = 1024,
		PRG_BANK_SIZE = 8192
	};

	uint8_t mode;
	bool modePrg;
	bool modeChr;
	uint16_t modeBanks[8];

	bool haveChrRam;
	uint8_t chrRam[8192];
	uint32_t lastPrgBank;

	bool irqEnabled;
	uint8_t irqLatchValue;
	uint8_t irqCounter;

	uint32_t prgAddrMask[4];
	uint32_t chrAddrMask[8];

	bool beforeLow;
	void fixBank();
	void processIRQ();
};

}
