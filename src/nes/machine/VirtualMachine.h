#pragma once

#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <tarte/XArchiver.h>

#include "debugger/Debugger.h"

#include "file/NesFile.h"
#include "fairy/VideoFairy.h"
#include "fairy/AudioFairy.h"
#include "fairy/GamepadFairy.h"
#include "AudioChannel.h"

namespace chisa{
namespace util {
class XArchiver;
class XArchiverIn;
class XArchiverOut;
}}

namespace nes {
using namespace tarte;

class VirtualMachine;

class Cartridge { //カートリッジデータ＋マッパー
public:
	enum{
		SRAM_SIZE = 8192
	};
	static Cartridge* loadCartridge(VirtualMachine& vm, std::string const& filename);
private:
	static Cartridge* loadCartridge(VirtualMachine& vm, NesFile* file);
public:
	explicit Cartridge(VirtualMachine& vm, const NesFile* nesFile = 0);
	virtual ~Cartridge();
	virtual void run(uint16_t clockDelta);
	virtual void onHardReset();
	virtual void onReset();

	/* from PPU */
	virtual uint8_t readPatternTableHigh(uint16_t addr) = 0;
	virtual void writePatternTableHigh(uint16_t addr, uint8_t val);
	virtual uint8_t readPatternTableLow(uint16_t addr) = 0;
	virtual void writePatternTableLow(uint16_t addr, uint8_t val);
	virtual uint8_t readNameTable(uint16_t addr);
	virtual void writeNameTable(uint16_t addr, uint8_t val);

	/* from CPU */
	virtual uint8_t readRegisterArea(uint16_t addr);
	virtual void writeRegisterArea(uint16_t addr, uint8_t val);
	virtual uint8_t debuggerReadRegisterArea(uint16_t addr);
	virtual void debuggerWriteRegisterArea(uint16_t addr, uint8_t val);

	virtual uint8_t readSaveArea(uint16_t addr);
	virtual void writeSaveArea(uint16_t addr, uint8_t val);
	virtual uint8_t debuggerReadSaveArea(uint16_t addr);
	virtual void debuggerWriteSaveArea(uint16_t addr, uint8_t val);

	virtual uint8_t readBankHigh(uint16_t addr) = 0;
	virtual void writeBankHigh(uint16_t addr, uint8_t val);
	virtual uint8_t debuggerReadBankHigh(uint16_t addr) = 0;
	virtual void debuggerWriteBankHigh(uint16_t addr, uint8_t val);

	virtual uint8_t readBankLow(uint16_t addr) = 0;
	virtual void writeBankLow(uint16_t addr, uint8_t val);
	virtual uint8_t debuggerReadBankLow(uint16_t addr) = 0;
	virtual void debuggerWriteBankLow(uint16_t addr, uint8_t val);

	void connectInternalVram(uint8_t* internalVram);
	void changeMirrorType(NesFile::MirrorType mirrorType);
protected:
	inline uint8_t readSram(uint16_t addr) const noexcept
	{
		return this->sram[addr & 0x1fff];
	}
	inline void writeSram(uint16_t addr, uint8_t value)
	{
		this->sram[addr & 0x1fff] = value;
	}
	void reserveIRQ();
	void releaseIRQ();
	const NesFile* const nesFile;
private:
	VirtualMachine& vm_;
	bool hasSram;
	uint8_t sram[SRAM_SIZE];
	NesFile::MirrorType mirrorType;
	uint8_t* vramMirroring[4];
	uint8_t* internalVram;
	uint8_t fourScreenVram[4096];
public: /* セーブ・ロード用 */
	static Cartridge* load(VirtualMachine& vm, XValue const& val);
	XValue save();
public: /* セーブ・ロード実装用 */
	virtual void saveImpl(XArchiverOut& arc) {};
	virtual void loadImpl(XArchiverIn& arc) {};
};

class IOPort final{
public:
	explicit IOPort(VirtualMachine& vm, GamepadFairy* pad1, GamepadFairy* pad2);
	~IOPort() noexcept = default;

	inline  void onVBlank(){
		pad1Fairy.onVBlank();
		pad2Fairy.onVBlank();
	}

	inline void writeOutReg(uint8_t const value)
	{
		if(((debugger_.ioWrite(value)) & 1) == 1){
			pad1Fairy.onUpdate();
			pad1Idx = 0;
			pad2Fairy.onUpdate();
			pad2Idx = 0;
		}
	}
	inline void debuggerWriteOutReg(uint8_t const value)
	{
		if((value & 1) == 1){
			pad1Fairy.onUpdate();
			pad1Idx = 0;
			pad2Fairy.onUpdate();
			pad2Idx = 0;
		}
	}
	inline uint8_t readInputReg1()
	{
		return debugger_.ioRead(0x4016, pad1Fairy.isPressed((pad1Idx++) & 7) ? 1 : 0);
	}
	inline uint8_t readInputReg2()
	{
		return debugger_.ioRead(0x4017, pad2Fairy.isPressed((pad2Idx++) & 7) ? 1 : 0);
	}
	inline uint8_t debuggerReadInputReg1()
	{
		return pad1Fairy.isPressed((pad1Idx) & 7) ? 1 : 0;
	}
	inline uint8_t debuggerReadInputReg2()
	{
		return pad2Fairy.isPressed((pad2Idx) & 7) ? 1 : 0;
	}


protected:
private:
	DummyGamepadFairy dummyPad;
	VirtualMachine& vm_;
	Debugger& debugger_;
	GamepadFairy& pad1Fairy;
	GamepadFairy& pad2Fairy;
	uint8_t pad1Idx;
	uint8_t pad2Idx;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & pad1Fairy;
		arc & pad2Fairy;
		arc & pad1Idx;
		arc & pad2Idx;
	}

};

class Audio final {
public:
	explicit Audio(VirtualMachine& vm, AudioFairy& audioFairy);
	~Audio() noexcept = default;
	void run(uint16_t clockDelta);
	void onHardReset();
	void onReset();
	uint8_t readReg(uint16_t addr);
	void writeReg(uint16_t addr, uint8_t value);
	uint8_t debuggerReadReg(uint16_t addr);
	void debuggerWriteReg(uint16_t addr, uint8_t value);
	void onVSync();
	enum{
		AUDIO_CLOCK = 21477272/12,//21.28MHz(NTSC)
		SAMPLE_RATE = 44100,
		FRAME_IRQ_RATE = 240
	};
protected:
private:
	VirtualMachine& vm_;
	Debugger& debugger_;
	AudioFairy& audioFairy;
	//
	unsigned int clockCnt;
	unsigned int leftClock;
	unsigned int frameCnt;
	//--
	bool isNTSCmode;
	bool frameIRQenabled;
	uint8_t frameIRQCnt;
	//---
	Rectangle rectangle1;
	Rectangle rectangle2;
	Triangle triangle;
	Noize noize;
	Digital digital;
	//---
	inline void analyzeStatusRegister(uint8_t value);
	inline void analyzeLowFrequentryRegister(uint8_t value);
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & audioFairy;
		arc & clockCnt;
		arc & leftClock;
		arc & frameCnt;
		arc & isNTSCmode;
		arc & frameIRQenabled;
		arc & frameIRQCnt;
		arc & rectangle1;
		arc & rectangle2;
		arc & triangle;
		arc & noize;
		arc & digital;
	}
};

class Video final{
public:
	explicit Video(VirtualMachine& vm, VideoFairy& videoFairy);
	~Video() noexcept = default;
	void run(uint16_t const clockDelta);
	void onHardReset();
	void onReset();
	uint8_t readReg(uint16_t const addr);
	void writeReg(uint16_t const addr, uint8_t const value);
	void executeDMA(uint8_t const value);
	void connectCartridge(Cartridge* const cartridge);
	enum{
		screenWidth = 256,
		screenHeight = 240,
		EmptyBit = 0x00,
		BackSpriteBit = 0x40,
		BackgroundBit = 0x80,
		FrontSpriteBit = 0xc0,
		SpriteLayerBit = 0x40,
		LayerBitMask = 0xc0
	};
protected:
private:
	enum{
		clockPerScanline = 341,
		scanlinePerScreen = 262,
		defaultSpriteCnt = 8
	};
	VirtualMachine& vm_;
	Debugger& debugger_;
	Cartridge* cartridge;
	VideoFairy& videoFairy;
	bool isEven;
	uint16_t nowY;
	uint16_t nowX;
	uint8_t spRam[256];
	uint8_t internalVram[2048];
	uint8_t palette[9][4];
	uint8_t screenBuffer[screenHeight][screenWidth];

	/* Rendering */
	struct SpriteSlot {
		uint8_t idx;
		uint8_t y;
		uint8_t x;
		uint8_t paletteNo;
		uint16_t tileAddr;
		bool isForeground;
		bool flipHorizontal;
		bool flipVertical;
	public:
		template <typename Archiver>
		void serialize(Archiver& arc){
			arc & idx;
			arc & y;
			arc & x;
			arc & paletteNo;
			arc & tileAddr;
			arc & isForeground;
			arc & flipHorizontal;
			arc & flipVertical;
		}
	} spriteTable[defaultSpriteCnt];
	uint16_t spriteHitCnt;
	inline void spriteEval();
	inline void buildSpriteLine();
	inline void buildBgLine();

	/* IO */
	inline uint8_t readVram(uint16_t const addr) const;
	inline void writeVram(uint16_t const addr, uint8_t const value);
	inline uint8_t readVramExternal(uint16_t const addr) const;
	inline void writeVramExternal(uint16_t const addr, uint8_t const value);
	inline uint8_t readPalette(uint16_t const addr) const;
	inline void writePalette(uint16_t const addr, uint8_t const value);
	inline uint8_t readSprite(uint16_t const addr) const;
	inline void writeSprite(uint16_t const addr, uint8_t const value);

	/* PPU Control Register 1 */
	bool executeNMIonVBlank;
	uint8_t spriteHeight;
	uint16_t patternTableAddressBackground;
	uint16_t patternTableAddress8x8Sprites;
	uint8_t vramIncrementSize;

	/* PPU Control Register 2 */
	uint8_t colorEmphasis;
	bool spriteVisibility;
	bool backgroundVisibility;
	bool spriteClipping;
	bool backgroundClipping;
	uint8_t paletteMask;

	/* PPU Status Register */
	bool nowOnVBnank;
	bool sprite0Hit;
	bool lostSprites;

	/* addressControl */
	uint8_t vramBuffer;
	uint8_t spriteAddr;
	uint16_t vramAddrRegister;
	uint16_t vramAddrReloadRegister;
	uint8_t horizontalScrollBits;
	bool scrollRegisterWritten;
	bool vramAddrRegisterWritten;

	inline void analyzePPUControlRegister1(uint8_t const value);
	inline void analyzePPUControlRegister2(uint8_t const value);
	inline void analyzePPUBackgroundScrollingOffset(uint8_t const value);
	inline uint8_t buildPPUStatusRegister();
	inline uint8_t readVramDataRegister();
	inline uint8_t readSpriteDataRegister();
	inline uint8_t debuggerBuildPPUStatusRegister();
	inline uint8_t debuggerReadVramDataRegister();
	inline uint8_t debuggerReadSpriteDataRegister();
	inline void analyzeVramAddrRegister(uint8_t const value);
	inline void analyzeSpriteAddrRegister(uint8_t const value);
	inline void writeVramDataRegister(uint8_t const value);
	inline void writeSpriteDataRegister(uint8_t const value);

	inline void startVBlank();
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & videoFairy;
		arc & isEven;
		arc & nowY;
		arc & nowX;
		arc & spRam;
		arc & internalVram;
		arc & palette;
		arc & screenBuffer;

		arc & spriteTable;
		arc & spriteHitCnt;

		/* PPU Control Register 1 */
		arc & executeNMIonVBlank;
		arc & spriteHeight;
		arc & patternTableAddressBackground;
		arc & patternTableAddress8x8Sprites;
		arc & vramIncrementSize;

		/* PPU Control Register 2 */
		arc & colorEmphasis;
		arc & spriteVisibility;
		arc & backgroundVisibility;
		arc & spriteClipping;
		arc & backgroundClipping;
		arc & paletteMask;

		/* PPU Status Register */
		arc & nowOnVBnank;
		arc & sprite0Hit;
		arc & lostSprites;

		/* addressControl */
		arc & vramBuffer;
		arc & spriteAddr;
		arc & vramAddrRegister;
		arc & vramAddrReloadRegister;
		arc & horizontalScrollBits;
		arc & scrollRegisterWritten;
		arc & vramAddrRegisterWritten;
	}
public:
	uint8_t debuggerReadReg(uint16_t const addr);
	void debuggerWriteReg(uint16_t const addr, uint8_t const value);
};

class Ram final {
public:
	enum{
		WRAM_LENGTH = 2048
	};
public:
	explicit Ram(VirtualMachine& vm);
	~Ram() noexcept = default;
	inline void onHardReset()
	{
		//from http://wiki.nesdev.com/w/index.php/CPU_power_up_state
		memset(wram_, 0xff, WRAM_LENGTH);
		wram_[0x8] = 0xf7;
		wram_[0x9] = 0xef;
		wram_[0xa] = 0xdf;
		wram_[0xf] = 0xbf;
	}
	inline void onReset()
	{
	}
	inline uint8_t read(uint16_t const addr)
	{
		return debugger_.ramRead(addr, wram_[addr & 0x7ff]);
	}
	inline void write(uint16_t const addr, uint8_t const value)
	{
		wram_[addr & 0x7ff] = debugger_.ramWrite(addr, value);
	}
protected:
private:
	VirtualMachine& vm_;
	Debugger& debugger_;
	uint8_t wram_[WRAM_LENGTH]; //2KB WRAM
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & wram_;
	}
public: /* debugger */
	inline uint8_t const (&wram() const noexcept)[WRAM_LENGTH] { return this->wram_; }
	inline uint8_t (&wram() noexcept)[WRAM_LENGTH] { return this->wram_; }
	inline uint8_t debuggerRead(uint16_t const addr) {
		return wram_[addr & 0x7ff];
	}
	inline void debuggerWrite(uint16_t const addr, uint8_t const value) {
		wram_[addr & 0x7ff] = value;
	}
};

class Processor
{
public:
	explicit Processor(VirtualMachine& vm);
	~Processor() noexcept = default;
	void run(uint16_t clockDelta);
	void onHardReset();
	void onReset();
	void sendNMI();
	void reserveIRQ();
	void releaseIRQ();
protected:
private:
	inline uint8_t read(uint16_t const addr);
	inline void write(uint16_t const addr, uint8_t const value);
	//定数
	enum{
		FLAG_C = 1,
		FLAG_Z = 2,
		FLAG_I = 4,
		FLAG_D = 8,
		FLAG_B = 16, //not used in NES
		FLAG_ALWAYS_SET = 32,
		FLAG_V = 64,
		FLAG_N = 128,
	};
	static const uint8_t ZNFlagCache[0x100];
	static const uint8_t CycleTable[0x100];
	//
	VirtualMachine& vm_;
	Debugger& debugger_;
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint16_t PC;
	uint8_t SP;
	uint8_t P;
	//
	bool NMI;
	bool IRQ;
	bool needStatusRewrite;
	uint8_t newStatus;
	//
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & A;
		arc & X;
		arc & Y;
		arc & PC;
		arc & SP;
		arc & P;
		arc & NMI;
		arc & IRQ;
		arc & needStatusRewrite;
		arc & newStatus;
	}
public:
	inline void push(uint8_t val);
	inline uint8_t pop();
	inline void consumeClock(uint8_t clock);
	inline void updateFlagZN(const uint8_t val);
	//ハードウェア☆割り込み
	inline void onNMI();
	inline void onIRQ();

	//アドレッシングモード
	inline uint16_t addrImmediate();
	inline uint16_t addrAbsolute();
	inline uint16_t addrZeroPage();
	inline uint16_t addrZeroPageIdxX();
	inline uint16_t addrZeroPageIdxY();
	inline uint16_t addrAbsoluteIdxX();
	inline uint16_t addrAbsoluteIdxY();
	inline uint16_t addrRelative();
	inline uint16_t addrIndirectX();
	inline uint16_t addrIndirectY();
	inline uint16_t addrAbsoluteIndirect();

	//命令一覧 from http://nesdev.parodius.com/opcodes.txt
	inline void BCC(uint16_t addr);
	inline void BCS(uint16_t addr);
	inline void BEQ(uint16_t addr);
	inline void BNE(uint16_t addr);
	inline void BVC(uint16_t addr);
	inline void BVS(uint16_t addr);
	inline void BPL(uint16_t addr);
	inline void BMI(uint16_t addr);

	inline void LDA(uint16_t addr);
	inline void LDY(uint16_t addr);
	inline void LDX(uint16_t addr);
	inline void STA(uint16_t addr);
	inline void STY(uint16_t addr);
	inline void STX(uint16_t addr);

	inline void TXA();
	inline void TYA();
	inline void TXS();
	inline void TAY();
	inline void TAX();
	inline void TSX();

	inline void PHP();
	inline void PLP();
	inline void PHA();
	inline void PLA();

	inline void ADC(uint16_t addr);
	inline void SBC(uint16_t addr);
	inline void CPX(uint16_t addr);
	inline void CPY(uint16_t addr);
	inline void CMP(uint16_t addr);

	inline void AND(uint16_t addr);
	inline void EOR(uint16_t addr);
	inline void ORA(uint16_t addr);
	inline void BIT(uint16_t addr);

	inline void ASL();
	inline void ASL(uint16_t addr);
	inline void LSR();
	inline void LSR(uint16_t addr);
	inline void ROL();
	inline void ROL(uint16_t addr);
	inline void ROR();
	inline void ROR(uint16_t addr);

	inline void INX();
	inline void INY();
	inline void INC(uint16_t addr);
	inline void DEX();
	inline void DEY();
	inline void DEC(uint16_t addr);

	inline void CLC();
	inline void CLI();
	inline void CLV();
	inline void CLD();
	inline void SEC();
	inline void SEI();
	inline void SED();

	inline void NOP();
	inline void BRK();

	inline void JSR(uint16_t addr);
	inline void JMP(uint16_t addr);
	inline void RTI();
	inline void RTS();
public:
	inline uint8_t getA() const noexcept { return A; };
	inline uint8_t getX() const noexcept { return X; };
	inline uint8_t getY() const noexcept { return Y; };
	inline uint16_t getPC() const noexcept { return PC; };
	inline uint8_t getSP() const noexcept { return SP; };
	inline uint8_t getP() const noexcept { return P; };
};

class VirtualMachine final
{
public:
	explicit VirtualMachine(VideoFairy& videoFairy, AudioFairy& audioFairy, GamepadFairy* player1, GamepadFairy* player2);
	~VirtualMachine();
	void run();
	void sendNMI(); //from video to processor
	enum{
		DEVICE_FRAMECNT = 1,
		DEVICE_DMC = 2,
		DEVICE_CARTRIDGE = 4
	};
	void reserveIRQ(uint8_t device); //from cartridge and audio to processor.
	void releaseIRQ(uint8_t device); //from cartridge and audio to processor.
	bool isIRQpending(uint8_t device);

	void sendVBlank(); //from video
	void sendHardReset(); //from user to all subsystems.
	void sendReset(); //from user to all subsystems.
	void loadCartridge(const char* filename);
	inline void consumeCpuClock(uint32_t clock)
	{
		consumeClock(clock * CPU_CLOCK_FACTOR);
	}
	inline uint8_t read(uint16_t const addr) //from processor to subsystems.
	{
		switch(addr & 0xE000){
			case 0x0000:
				return ram_.read(addr);
			case 0x2000:
				return video_.readReg(addr);
			case 0x4000:
				//このへんは込み入ってるので、仕方ないからここで振り分け。
				if(addr == 0x4015){
					return audio_.readReg(addr);
				}else if(addr == 0x4016){
					return ioPort_.readInputReg1();
				}else if(addr == 0x4017){
					return ioPort_.readInputReg2();
				}else if(addr < 0x4018){
					throw EmulatorException("[FIXME] Invalid addr: 0x") << std::hex << addr;
				}else{
					return debugger_.cartridgeReadCPUReg(addr, cartridge_->readRegisterArea(addr));
				}
			case 0x6000:
				return debugger_.cartridgeReadCPUSaveArea(addr, cartridge_->readSaveArea(addr));
			case 0x8000:
			case 0xA000:
				return debugger_.cartridgeReadCPUBankLow(addr, cartridge_->readBankLow(addr));
			case 0xC000:
			case 0xE000:
				return debugger_.cartridgeReadCPUBankHigh(addr, cartridge_->readBankHigh(addr));
			default:
				throw EmulatorException("[FIXME] Invalid addr: 0x") << std::hex << addr;
		}
	}
	inline uint8_t debuggerRead(uint16_t const addr) //from processor to subsystems.
	{
		switch(addr & 0xE000){
			case 0x0000:
				return ram_.debuggerRead(addr);
			case 0x2000:
				return video_.debuggerReadReg(addr);
			case 0x4000:
				//このへんは込み入ってるので、仕方ないからここで振り分け。
				if(addr == 0x4015){
					return audio_.debuggerReadReg(addr);
				}else if(addr == 0x4016){
					return ioPort_.debuggerReadInputReg1();
				}else if(addr == 0x4017){
					return ioPort_.debuggerReadInputReg2();
				}else if(addr < 0x4018){
					return 0;
				}else{
					return cartridge_->debuggerReadRegisterArea(addr); //XXX: 副作用避け
				}
			case 0x6000:
				return cartridge_->debuggerReadSaveArea(addr); //XXX: 副作用避け
			case 0x8000:
			case 0xA000:
				return cartridge_->debuggerReadBankLow(addr); //XXX: 副作用避け
			case 0xC000:
			case 0xE000:
				return cartridge_->debuggerReadBankHigh(addr); //XXX: 副作用避け
			default:
				return 0;
		}
	}
	inline void write(uint16_t const addr, uint8_t const value) // from processor to subsystems.
	{
		switch(addr & 0xE000){
			case 0x0000:
				ram_.write(addr, value);
				break;
			case 0x2000:
				video_.writeReg(addr, value);
				break;
			case 0x4000:
				if(addr == 0x4014){
					video_.executeDMA(value);
				}else if(addr == 0x4016){
					ioPort_.writeOutReg(value);
				}else if(addr < 0x4018){
					audio_.writeReg(addr, value);
				}else{
					cartridge_->writeRegisterArea(addr, debugger_.cartridgeWriteCPUReg(addr, value));
				}
				break;
			case 0x6000:
				cartridge_->writeSaveArea(addr, debugger_.cartridgeWriteCPUSaveArea(addr, value));
				break;
			case 0x8000:
			case 0xA000:
				cartridge_->writeBankLow(addr, debugger_.cartridgeWriteCPUBankLow(addr, value));
				break;
			case 0xC000:
			case 0xE000:
				cartridge_->writeBankHigh(addr, debugger_.cartridgeWriteCPUBankHigh(addr, value));
				break;
			default:
				throw EmulatorException("[FIXME] Invalid addr: 0x") << std::hex << addr;
		}
	}
	inline void debuggerWrite(uint16_t const addr, uint8_t const value) // from debugger.
	{
		switch(addr & 0xE000){
			case 0x0000:
				ram_.debuggerWrite(addr, value);
				break;
			case 0x2000:
				video_.debuggerWriteReg(addr, value);
				break;
			case 0x4000:
				if(addr == 0x4014){
					video_.executeDMA(value); //FIXME: どうする？？
				}else if(addr == 0x4016){
					ioPort_.debuggerWriteOutReg(value);
				}else if(addr < 0x4018){
					audio_.debuggerWriteReg(addr, value);
				}else{
					cartridge_->debuggerWriteRegisterArea(addr, value); //XXX: 副作用避け
				}
				break;
			case 0x6000:
				cartridge_->debuggerWriteSaveArea(addr, value); //XXX: 副作用避け
				break;
			case 0x8000:
			case 0xA000:
				cartridge_->debuggerWriteBankLow(addr, value); //XXX: 副作用避け
				break;
			case 0xC000:
			case 0xE000:
				cartridge_->debuggerWriteBankHigh(addr, value); //XXX: 副作用避け
				break;
			default:
				break;
		}
	}
private:
	enum{
		MAIN_CLOCK = 21477272, //21.28MHz(NTSC)
		CPU_CLOCK_FACTOR = 12,
		VIDEO_CLOCK_FACTOR = 4,
	};
	void consumeClock(uint32_t clock);
private:
	Debugger debugger_;
private:
	Ram ram_;
	Processor processor_;
	Audio audio_;
	Video video_;
	Cartridge* cartridge_;
	IOPort ioPort_;

	uint32_t clockDelta;

	bool resetFlag;
	bool hardResetFlag;

	uint8_t irqLine;
public: /* Save/Load */
	XValue save();
	void load(XValue const& data);
public: /* Debugger */
	inline Processor const& processor() const noexcept { return processor_; };
	inline Ram& ram() noexcept{ return this->ram_; }
	inline Ram const& ram() const noexcept{ return this->ram_; }
	inline Debugger& debugger() noexcept { return this->debugger_; };
	inline Debugger const& debugger() const noexcept { return this->debugger_; };
	void onBreak() noexcept;
};

}
