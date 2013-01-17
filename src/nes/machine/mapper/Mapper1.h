#pragma once
#include "../VirtualMachine.h"
#include "../file/NesFile.h"

namespace nes {

class Mapper1 : public Cartridge
{
	public:
		Mapper1(VirtualMachine& vm, const NesFile* nesFile);
		virtual ~Mapper1();
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
	protected:
	private:
		const bool is512krom;
		bool useHighPrgBank;
		const bool hasChrRam;
		uint8_t chrRam[8192];

		const uint16_t lastPrgBank;
		uint8_t chrMode;
		uint8_t prgMode;

		uint8_t highChrBank;
		uint8_t lowChrBank;
		uint8_t prgBank;

		uint32_t highPrgAddrBase;
		uint32_t lowPrgAddrBase;
		uint32_t highChrAddrBase;
		uint32_t lowChrAddrBase;

		uint8_t reg;
		uint8_t regCnt;

		inline void updateBank();
};

}
