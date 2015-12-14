/*
 * Mapper3.cpp
 *
 *  Created on: 2011/09/05
 *      Author: psi
 */

#include "Mapper3.hpp"

namespace nes {

Mapper3::Mapper3(VirtualMachine& vm, const NesFile* nesFile) :
Cartridge(vm, nesFile),
prgHighBankAddrBase((nesFile->getPrgPageCnt()-1)*NesFile::PRG_ROM_PAGE_SIZE),
chrBankAddrBase(0)
{
}

Mapper3::~Mapper3()
{
}

uint8_t Mapper3::readPatternTableHigh(uint16_t addr)
{
	return this->nesFile->readChr(chrBankAddrBase | (addr & 0x1fff));
}
uint8_t Mapper3::readPatternTableLow(uint16_t addr)
{
	return this->nesFile->readChr(chrBankAddrBase | (addr & 0x1fff));
}

/* for CPU */
uint8_t Mapper3::readBankHigh(uint16_t addr)
{
	return nesFile->readPrg(prgHighBankAddrBase | (addr & 0x3fff));
}
uint8_t Mapper3::debuggerReadBankHigh(uint16_t addr)
{
	return readBankHigh(addr);
}
void Mapper3::debuggerWriteBankHigh(uint16_t addr, uint8_t val){
	nesFile->writePrg(prgHighBankAddrBase | (addr & 0x3fff), val);
}
void Mapper3::writeBankHigh(uint16_t addr, uint8_t val)
{
	writeReg(addr, val);
}
uint8_t Mapper3::readBankLow(uint16_t addr)
{
	return nesFile->readPrg(addr & 0x3fff);
}
uint8_t Mapper3::debuggerReadBankLow(uint16_t addr)
{
	return readBankLow(addr);
}
void Mapper3::debuggerWriteBankLow(uint16_t addr, uint8_t val)
{
	nesFile->writePrg(addr & 0x3fff, val);
}
void Mapper3::writeBankLow(uint16_t addr, uint8_t val)
{
	writeReg(addr, val);
}

inline void Mapper3::writeReg(uint16_t addr, uint8_t val)
{
	chrBankAddrBase = (val & 3) * NesFile::CHR_ROM_PAGE_SIZE;
}

}
