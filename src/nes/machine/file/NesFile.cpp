
#include <fstream>
#include <string.h>
#include <tarte/XArchiver.h>
#include "./NesFile.h"
#include "../exception/EmulatorException.h"

namespace nes {

NesFile::NesFile(std::string const& filename) :
filename(filename),
mapperNo(0),
prgRom(nullptr),
chrRom(nullptr),
mirrorType(HORIZONTAL),
trainerFlag(false),
sramFlag(false),
prgSize(0),
chrSize(0),
prgPageCnt(0),
chrPageCnt(0)
{
	this->loadFile();
}

NesFile::NesFile(tarte::XValue const& val) :
filename("<FROM XVAL NOW LOADING>"),
mapperNo(0),
prgRom(nullptr),
chrRom(nullptr),
mirrorType(HORIZONTAL),
trainerFlag(false),
sramFlag(false),
prgSize(0),
chrSize(0),
prgPageCnt(0),
chrPageCnt(0)
{
	::tarte::XArchiverIn in(val);
	in >> *this;
}

NesFile::~NesFile()
{
	if(this->chrRom){
		delete [] this->chrRom;
	}
	if(this->prgRom){
		delete [] this->prgRom;
	}
}

void NesFile::loadFile()
{
	std::ifstream in(filename, std::fstream::binary);
	in.seekg(0, std::ifstream::end);
	const std::ifstream::pos_type endPos = in.tellg();
	in.seekg(0, std::ifstream::beg);
	const std::ifstream::pos_type startPos = in.tellg();
	const uint32_t size = static_cast<uint32_t>(endPos - startPos);

	uint8_t* const data = new uint8_t[size];
	try{
		in.read(reinterpret_cast<char*>(data), size);
		if(in.gcount() != static_cast<int32_t>(size)){
			throw EmulatorException("[FIXME] Invalid file format: ") << filename;
		}
		const uint32_t contentSize = size - NES_FORMAT_SIZE;
		this->analyzeFile(data, contentSize, &data[NES_FORMAT_SIZE]);
		delete [] data;
	} catch (...) {
		delete [] data;
		throw;
	}
}

void NesFile::analyzeFile(const uint8_t* const header, const uint32_t filesize, const uint8_t* data)
{
	if(!(header[0] == 'N' && header[1]=='E' && header[2]=='S' && header[3] == 0x1a)){
		throw EmulatorException("[FIXME] Invalid header: ") << filename;
	}
	this->prgSize = PRG_ROM_PAGE_SIZE * header[4];
	this->chrSize = CHR_ROM_PAGE_SIZE * header[5];
	this->prgPageCnt = header[4];
	this->chrPageCnt = header[5];
	this->mapperNo = ((header[6] & 0xf0)>>4) | (header[7] & 0xf0);
	this->trainerFlag = (header[6] & 0x4) == 0x4;
	this->sramFlag = (header[6] & 0x2) == 0x2;
	if((header[6] & 0x8) == 0x8){
		this->mirrorType = FOUR_SCREEN;
	}else{
		this->mirrorType = (header[6] & 0x1) == 0x1 ? VERTICAL : HORIZONTAL;
	}

	uint32_t fptr = 0;
	if(this->trainerFlag){
		if(fptr + TRAINER_SIZE > filesize){
			throw EmulatorException("[FIXME] Invalid file size; too short!: ") << filename;
		}
		memcpy(this->trainer, &data[fptr], TRAINER_SIZE);
		fptr += TRAINER_SIZE;
	}
	uint8_t* prgRom = new uint8_t[this->prgSize];
	if(fptr + this->prgSize > filesize){
		throw EmulatorException("[FIXME] Invalid file size; too short!: ") << filename;
	}
	memcpy(prgRom, &data[fptr], this->prgSize);
	fptr += this->prgSize;
	this->prgRom = prgRom;

	uint8_t* chrRom = new uint8_t[this->chrSize];
	if(fptr + this->chrSize > filesize){
		throw EmulatorException("[FIXME] Invalid file size; too short!: ") << filename;
	}else if(fptr + this->chrSize < filesize){
		throw EmulatorException("[FIXME] Invalid file size; too long!: ") << filename;
	}
	memcpy(chrRom, &data[fptr], this->chrSize);
	fptr += this->chrSize;
	this->chrRom = chrRom;
}

}
