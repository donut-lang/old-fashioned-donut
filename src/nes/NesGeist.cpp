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
#include "../chisa/gl/DrawableManager.h"
#include "../chisa/input/JoystickManager.h"
#include "Hexe.h"

namespace nes {

NesGeist::NesGeist(chisa::Logger& log, ::tarte::Handler<Hexe> const& hexe, ::tarte::HandlerW<chisa::tk::World> world)
:chisa::WorldGeist(log, hexe, world)
,machine_(nullptr)
,runner_t_(nullptr)
,runner_(nullptr)
,time_ms_(0.0f)
,video_(*this)
,audio_(*this)
,gamepad_(this->world()->joystickManager())
{
	using namespace chisa::gl;
	this->machine_ = new VirtualMachine(video_, audio_, &gamepad_, nullptr);
	if( chisa::Handler<chisa::tk::World> world = this->world_.lock() ){
		this->spr_ = world->drawableManager()->queryRawSprite(ImageFormat::RGBA8, 256, 240);
	}
	this->world()->quartet()->addInstrument(this->audio_.instrument());
}

NesGeist::~NesGeist() noexcept
{
	if(Handler< ::chisa::tk::World> world = this->world()){
		if(Handler< ::chisa::Quartet> quartet = world->quartet()){
			quartet->removeInstrument(this->audio_.instrument());
		}
	}
	this->stopNES();
	delete this->machine_;
}

Handler<Hexe> NesGeist::hexe() {
	return ::tarte::Handler<Hexe>::__internal__fromRawPointerWithoutCheck(static_cast<Hexe*>(WorldGeist::hexe().get()));
}

Handler< ::donut::Object> NesGeist::createDonutObject(Handler< ::donut::Heap> const& heap)
{
	return hexe()->nesGeistProvider()->newInstance(heap, self());
}

std::string NesGeist::toString() const
{
	return format("(NesGeist %p)", this);
}

void NesGeist::loadNES(std::string const& abs_filename)
{
	this->stopNES();
	this->machine_->loadCartridge(abs_filename.c_str());
	this->machine_->sendHardReset();
}
void NesGeist::stopNES()
{
	if(this->runner_){
		this->runner_->queryStop();
		this->runner_t_->join();
		delete this->runner_t_;
		delete this->runner_;
		this->runner_t_ = nullptr;
		this->runner_ = nullptr;
	}
}

void NesGeist::startNES()
{
	this->stopNES();
	this->runner_ = new Runner(*this);
	this->runner_t_ = new std::thread(std::ref(*this->runner_));
	this->time_ms_ = 0.0f;
	if( chisa::Handler<chisa::tk::World> world = this->world() ) {
		world->sendTask([this](float delta_ms)->bool{
			std::unique_lock<std::mutex> lock(this->frame_mutex_);
			this->time_ms_ += delta_ms;
			this->gamepad_.updateJoystick();
			while(this->time_ms_ > (1.0f/60)){
				this->time_ms_ -= (1.0f/60);
				this->cond_.notify_one();
			}
			return (this->runner_);
		});
	}
}

XValue NesGeist::saveNES()
{
	this->stopNES();
	XValue const savedata = this->machine_->save();
	this->startNES();
	return savedata;
}

void NesGeist::loadNES(XValue const& data)
{
	this->stopNES();
	this->machine_->load(data);
	this->startNES();
}

//---------------------------------------------------------------------------------------------------------------------
// 実行スレッドの実装
//---------------------------------------------------------------------------------------------------------------------

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
	while( likely(!this->stop_) ){
		for(int x=100; x>0;--x){
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

