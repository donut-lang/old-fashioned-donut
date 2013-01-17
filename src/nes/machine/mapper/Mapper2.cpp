/*
 * Mapper2.cpp
 *
 *  Created on: 2011/09/13
 *      Author: psi
 */

#include "Mapper2.h"
#include <string.h>

namespace nes {

Mapper2::Mapper2(VirtualMachine & vm, const NesFile *nesFile):
Cartridge(vm, nesFile),
lowBankAddrBase(0),
highBankAddrBase(NesFile::PRG_ROM_PAGE_SIZE * (nesFile->getPrgPageCnt()-1))
{
	memset(vram, 0 , 8192);
}

Mapper2::~Mapper2()
{
}

uint8_t Mapper2::readPatternTableHigh(uint16_t addr)
{
	return vram[(addr & 0xfff) | 0x1000];
}
void Mapper2::writePatternTableHigh(uint16_t addr, uint8_t val)
{
	vram[(addr & 0xfff) | 0x1000] = val;
}

uint8_t Mapper2::readPatternTableLow(uint16_t addr)
{
	return vram[addr & 0xfff];
}
void Mapper2::writePatternTableLow(uint16_t addr, uint8_t val)
{
	vram[addr & 0xfff] = val;
}


uint8_t Mapper2::readBankHigh(uint16_t addr)
{
	return nesFile->readPrg((addr & 0x3fff) | highBankAddrBase);
}

uint8_t Mapper2::debuggerReadBankHigh(uint16_t addr)
{
	return readBankHigh(addr);
}

void Mapper2::writeBankHigh(uint16_t addr, uint8_t val)
{
	lowBankAddrBase = NesFile::PRG_ROM_PAGE_SIZE * (val & 0xf);
}

void Mapper2::debuggerWriteBankHigh(uint16_t addr, uint8_t val)
{
	return nesFile->writePrg((addr & 0x3fff) | highBankAddrBase, val);
}

uint8_t Mapper2::readBankLow(uint16_t addr)
{
	return nesFile->readPrg((addr & 0x3fff) | lowBankAddrBase);
}
uint8_t Mapper2::debuggerReadBankLow(uint16_t addr)
{
	return readBankLow(addr);
}
void Mapper2::debuggerWriteBankLow(uint16_t addr, uint8_t val)
{
	return nesFile->writePrg((addr & 0x3fff) | lowBankAddrBase, val);
}

void Mapper2::writeBankLow(uint16_t addr, uint8_t val)
{
	lowBankAddrBase = NesFile::PRG_ROM_PAGE_SIZE * (val & 15);
}

}
