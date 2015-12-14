/*
 * NesFile.h
 *
 *  Created on: 2011/08/23
 *	  Author: psi
 */

#pragma once
#include <cstddef>
#include <stdint.h>
#include <stdio.h>
#include <cinamo/XVal.h>

namespace nes {

class NesFile
{
public:
	enum MirrorType {SINGLE0, SINGLE1, HORIZONTAL, VERTICAL, FOUR_SCREEN};
	enum{
		NES_FORMAT_SIZE = 16,
		TRAINER_SIZE = 512,
		PRG_ROM_PAGE_SIZE = 16 * 1024,
		CHR_ROM_PAGE_SIZE = 8 * 1024,
	};
public:
	explicit NesFile(std::string const& filename);
	explicit NesFile(cinamo::XValue const& val);
	~NesFile();
	inline MirrorType getMirrorType() const
	{
			return this->mirrorType;
	}
	inline bool hasTrainer() const
	{
		return this->trainerFlag;
	}
	inline bool hasSram() const
	{
		return this->sramFlag;
	}
	inline uint8_t getMapperNo() const
	{
		return this->mapperNo;
	}
	inline uint32_t getPrgSize() const
	{
		return this->prgSize;
	}
	inline uint32_t getPrgPageCnt() const
	{
		return this->prgPageCnt;
	}
	inline uint32_t getChrSize() const
	{
		return this->chrSize;
	}
	inline uint32_t getChrPageCnt() const
	{
		return this->chrPageCnt;
	}
	inline uint8_t readTrainer(uint16_t addr) const
	{
		return this->trainer[addr];
	}
	inline uint8_t readPrg(uint32_t addr) const
	{
		return this->prgRom[addr];
	}
	inline uint8_t readChr(uint32_t addr) const
	{
		return this->chrRom[addr];
	}
public: /* XXX: for debug 実際にはこれは中間オブジェクトにしておくべきなのでは。 */
	inline void writePrg(uint32_t addr, uint8_t const& val) const
	{
		const_cast<uint8_t&>(this->prgRom[addr]) = val;
	}
private:
	const std::string filename;
	uint8_t mapperNo;
	const uint8_t* prgRom;
	const uint8_t* chrRom;
	uint8_t trainer[512];

	MirrorType mirrorType;
	bool trainerFlag;
	bool sramFlag;
	uint32_t prgSize;
	uint32_t chrSize;
	uint32_t prgPageCnt;
	uint32_t chrPageCnt;

	void loadFile();
	void analyzeFile(const uint8_t* const header, const uint32_t filesize, const uint8_t* data);
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & const_cast<std::string&>(filename);
		arc & mapperNo;
		arc & trainer;
		arc & mirrorType;
		arc & trainerFlag;
		arc & sramFlag;
		arc & prgSize;
		arc & chrSize;
		arc & prgPageCnt;
		arc & chrPageCnt;
		if(!prgRom){
			prgRom = new uint8_t[prgSize];
		}
		arc.binary(const_cast<uint8_t*>(prgRom), prgSize);
		if(!chrRom){
			chrRom = new uint8_t[chrSize];
		}
		arc.binary(const_cast<uint8_t*>(chrRom), chrSize);
	}
};

}
