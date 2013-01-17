#pragma once

#include "../VirtualMachine.h"
#include "../file/NesFile.h"

namespace nes {


class Mapper0 : public Cartridge
{
	public:
		Mapper0(VirtualMachine& vm, const NesFile* nesFile);
		virtual ~Mapper0();

		/* for PPU */
		virtual uint8_t readPatternTableHigh(uint16_t addr) override final;
		virtual void writePatternTableHigh(uint16_t addr, uint8_t val) override final;

		virtual uint8_t readPatternTableLow(uint16_t addr) override final;
		virtual void writePatternTableLow(uint16_t addr, uint8_t val) override final;

		/* for CPU */
		virtual uint8_t readBankHigh(uint16_t addr) override final;
		virtual void writeBankHigh(uint16_t addr, uint8_t val) override final;
		virtual uint8_t readBankLow(uint16_t addr) override final;
		virtual void writeBankLow(uint16_t addr, uint8_t val) override final;

		virtual uint8_t debuggerReadBankHigh(uint16_t addr) override final;
		virtual void debuggerWriteBankHigh(uint16_t addr, uint8_t val) override final;
		virtual uint8_t debuggerReadBankLow(uint16_t addr) override final;
		virtual void debuggerWriteBankLow(uint16_t addr, uint8_t val) override final;
protected:
	private:
		const uint16_t addrMask;
		const bool hasChrRam;
		uint8_t chrRam[8192];
	public:
		template <typename Archiver>
		void serialize(Archiver& arc){
			arc & const_cast<uint16_t&>(addrMask);
			arc & const_cast<bool&>(hasChrRam);
			arc & chrRam;
		}
};

}
