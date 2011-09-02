/*
 * NesFile.h
 *
 *  Created on: 2011/08/23
 *      Author: psi
 */

#ifndef NESFILE_H_
#define NESFILE_H_
#include <cstddef>
#include <stdint.h>

class NesFile
{
    public:
        enum MirrorType {SINGLE, HORIZONTAL, VERTICAL, FOUR_SCREEN};
        static const uint16_t NES_FORMAT_SIZE = 16;
        static const uint16_t TRAINER_SIZE = 512;
        static const uint16_t PRG_ROM_PAGE_SIZE = 16 * 1024;
        static const uint16_t CHR_ROM_PAGE_SIZE = 8 * 1024;
    public:
        explicit NesFile(const char* filename);
        explicit NesFile(const uint32_t filesize, const uint8_t* data);
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
        inline uint8_t readPrg(uint16_t page, uint16_t addr) const
        {
            return this->readPrg(page * PRG_ROM_PAGE_SIZE + addr);
        }
        inline uint8_t readPrg(uint32_t addr) const
        {
            return this->prgRom[addr];
        }
        inline uint8_t readChr(uint16_t page, uint16_t addr) const
        {
            return this->readChr(page * PRG_ROM_PAGE_SIZE + addr);
        }
        inline uint8_t readChr(uint32_t addr) const
        {
            return this->chrRom[addr];
        }
    private:
        const std::string& filename;
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

        void analyzeFile(const uint8_t* const header, const uint32_t filesize, const uint8_t* data);
};

#endif /* NESFILE_H_ */
