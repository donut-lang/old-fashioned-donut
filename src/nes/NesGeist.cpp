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

static const std::string TAG("NesGeist");

NesGeist::NesGeist(chisa::Logger& log, ::tarte::Handler<Hexe> const& hexe, ::tarte::HandlerW<chisa::tk::World> world)
:chisa::WorldGeist(log, hexe, world)
,machine_(nullptr)
,running_(false)
,runner_t_(nullptr)
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
	log().d(TAG, "Shutting down...");

	if(Handler< ::chisa::Quartet> quartet = audio_.instrument()->quartet()){
		quartet->removeInstrument(this->audio_.instrument());
		log().d(TAG, "Audio fairy detatched.");
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
	if(this->runner_t_){
		this->queryStop();
		this->runner_t_->join();
		delete this->runner_t_;
		this->runner_t_ = nullptr;
	}
}

void NesGeist::startNES()
{
	this->stopNES();
	this->running_ = true;
	this->runner_t_ = new std::thread(std::ref(*this));
	this->time_ms_ = 0.0f;
	if( chisa::Handler<chisa::tk::World> world = this->world() ) {
		world->sendTask([this](float delta_ms)->bool{
			std::unique_lock<std::mutex> lock(this->cond_mutex_);
			this->time_ms_ += delta_ms;
			this->gamepad_.updateJoystick();
			while(this->time_ms_ > (1.0f/60)){
				this->time_ms_ -= (1.0f/60);
				this->cond_.notify_one();
			}
			return (this->runner_t_);
		});
	}
}

XValue NesGeist::saveNES()
{
	XValue savedata;
	{
		MachineLock lock(*this);
		savedata = this->machine_->save();
	}
	return savedata;
}

void NesGeist::loadNES(XValue const& data)
{
	MachineLock lock(*this);
	this->machine_->load(data);
}

//---------------------------------------------------------------------------------------------------------------------
// 実行スレッドの実装
//---------------------------------------------------------------------------------------------------------------------

void NesGeist::queryStop()
{
	this->running_ = false;
}

void NesGeist::operator ()()
{
	MachineLock l(*this);
	while( likely(this->running_) ){
		for(int x=100; x>0;--x){
			this->machine_->run();
		}
	}
}
}

