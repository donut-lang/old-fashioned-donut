/**
 * Chisa
 * Copyright (C) 2012 psi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NesGeist.h"

namespace nes {

NesGeist::NesGeist(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> world)
:chisa::WorldGeist(log, world)
{
	this->machine_ = new VirtualMachine(*this, *this, this, nullptr);
}

NesGeist::~NesGeist()
{
	this->queryStop();
	this->join();
}

std::string NesGeist::toString() const
{
	return chisa::util::format("(NesGeist %p)", this);
}

void NesGeist::dispatchRendering(const uint8_t nesBuffer[screenHeight][screenWidth], const uint8_t paletteMask)
{
	chisa::util::RWLock::WriteLock(this->sprLock_);
	{
		chisa::gl::RawSprite::Session s(this->sprA_);
		unsigned char* mem8 = s.data();
		unsigned int* mem32 = nullptr;
		const int stride = s.stride();
		for(int y=0;y<screenHeight;++y){
			mem32 = reinterpret_cast<unsigned int*>(mem8);
			for(int x=0;x<screenHeight;++x){
				mem32[x] = nesPaletteARGB[nesBuffer[y][x] & paletteMask];
			}
			mem8+=stride;
		}
	}
}

void NesGeist::onUpdate()
{
}

bool NesGeist::isPressed(uint8_t keyIdx)
{
	return false;
}

void NesGeist::run()
{
	while(!this->isStopQueried()){
		for(size_t i=0;i<1000*1000;++i){
			this->machine_->run();
		}
	}
}

void NesGeist::loadNES(const std::string& abs_filename)
{
	this->machine_->loadCartridge(abs_filename.c_str());
}

void NesGeist::startNES()
{
	this->start();
}

}
