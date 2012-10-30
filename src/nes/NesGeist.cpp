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
#include "../chisa/tk/World.h"

namespace nes {

NesGeist::NesGeist(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> world)
:chisa::WorldGeist(log, world)
,chisa::tk::Task(log)
,machine_(nullptr)
,runner_t_(nullptr)
,runner_(nullptr)
,time_ms_(0.0f)
{
	this->machine_ = new VirtualMachine(*this, *this, this, nullptr);
	if( std::shared_ptr<chisa::tk::World> world = this->world_.lock() ){
		this->spr_ = world->queryRawSprite(256, 240);
	}
}

NesGeist::~NesGeist()
{
}

std::string NesGeist::toString() const
{
	return chisa::util::format("(NesGeist %p)", this);
}

void NesGeist::dispatchRendering(const uint8_t nesBuffer[screenHeight][screenWidth], const uint8_t paletteMask)
{
	{
		NesGeist::Lock lock(*this);
		using namespace chisa::gl;
		RawSprite::Session s(lock.getSprite(), RawSprite::BufferType::RGBA8);
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
	{
		std::unique_lock<std::mutex> lock(this->frame_mutex_);
		if(this->runner_){
			this->cond_.wait(lock);
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

void NesGeist::loadNES(const std::string& abs_filename)
{
	this->stopNES();
	this->machine_->loadCartridge(abs_filename.c_str());
	this->machine_->sendHardReset();
}
void NesGeist::stopNES()
{
	if(this->runner_){
		delete this->runner_t_;
		delete this->runner_;
		this->runner_t_ = nullptr;
		this->runner_ = nullptr;
		this->world()->unregisterTask(this);
	}
}

bool NesGeist::exec(const float delta_ms)
{
	std::unique_lock<std::mutex> lock(this->frame_mutex_);
	this->time_ms_ += delta_ms;
	while(this->time_ms_ > (1.0f/60)){
		this->time_ms_ -= (1.0f/60);
		this->cond_.notify_one();
	}
	return true;
}

void NesGeist::startNES()
{
	this->stopNES();
	this->runner_ = new Runner(*this);
	this->runner_t_ = new std::thread(std::ref(*this->runner_));
	this->time_ms_ = 0.0f;
	this->world()->registerTask(this);
}

NesGeist::Runner::Runner(NesGeist& parent)
:parent_(parent)
,stop_(false)
{

}

void NesGeist::Runner::queryStop()
{
	this->stop_=true;
}

void NesGeist::Runner::operator ()()
{
	while(!this->stop_){
		for(size_t i=0;i<1000*1000;++i){
			this->parent_.machine_->run();
		}
	}
}

NesGeist::Lock::Lock(NesGeist& parent)
:parent_(parent)
{
	parent_.spr_mutex_.lock();
}
NesGeist::Lock::~Lock()
{
	parent_.spr_mutex_.unlock();
}

}

