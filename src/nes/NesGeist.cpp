/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "NesGeist.hpp"
#include "../chisa/tk/World.hpp"
#include "../chisa/gl/DrawableManager.hpp"
#include "../chisa/input/JoystickManager.hpp"
#include "Hexe.hpp"

namespace nes {

static const std::string TAG("NesGeist");

NesGeist::NesGeist(chisa::Logger& log, ::cinamo::Handler<Hexe> const& hexe, ::cinamo::HandlerW<chisa::tk::World> world)
:chisa::WorldGeist(log, hexe, world)
,machine_(nullptr)
,running_(false)
,runner_t_(nullptr)
,time_ms_(0.0f)
,isBreak_(false)
,video_(*this)
,audio_(*this)
,gamepad_(*this, this->world()->joystickManager())
{
	using namespace chisa::gl;
	this->machine_ = new VirtualMachine(*this, video_, audio_, &gamepad_, nullptr);
	if( chisa::Handler<chisa::tk::World> world = this->world_.lock() ){
		this->spr_ = world->drawableManager()->queryRawSprite(ImageFormat::RGBA8, 256, 240);
	}
}

NesGeist::~NesGeist() noexcept
{
	log().d(TAG, "Shutting down...");

	log().d(TAG, "Sending stop request to emulator...");
	this->stopNES();
	log().d(TAG, "Emulator stopped.");
	delete this->machine_;
}

Handler<Hexe> NesGeist::hexe() {
	return ::cinamo::Handler<Hexe>::__internal__fromRawPointerWithoutCheck(static_cast<Hexe*>(WorldGeist::hexe().get()));
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
		this->machine_->debugger().watcher().resumeRunning(); //XXX:再開してから死なないとデッドロックする
		if(Handler< ::chisa::Quartet> quartet = audio_.instrument()->quartet()){
			log().d(TAG, "Audio fairy detaching....");
			quartet->removeInstrument(this->audio_.instrument());
			log().d(TAG, "Audio fairy detached.");
		}
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
	this->world()->quartet()->addInstrument(this->audio_.instrument());
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

void NesGeist::onBreak()
{
	MachineUnlock unlock(*this);
	{
		std::unique_lock<std::mutex> lock(breakMutex_);
		isBreak_=true;
		breakCond_.wait(lock);
		isBreak_=false;
	}
}
void NesGeist::onStep()
{
	{
		std::unique_lock<std::mutex> lock(breakMutex_);
		breakCond_.notify_all();
	}
}

void NesGeist::loadNES(XValue const& data)
{
	this->machine()->debugger().watcher().removeMemoryExecBreakAll();
	onStep();
	{
		MachineLock lock(*this);
		this->machine_->load(data);
	}
}

//---------------------------------------------------------------------------------------------------------------------
// 実行スレッドの実装
//---------------------------------------------------------------------------------------------------------------------

void NesGeist::queryStop()
{
	this->running_ = false;
}

void NesGeist::run()
{
	while( likely(this->running_) ){
		for(int x=100; x>0;--x){
			this->machine_->run();
		}
	}
}

void NesGeist::operator ()()
{
	MachineLock l(*this);
	while( likely(this->running_) ){
		try {
			run();
		} catch (...) {
			this->machine_->debugger().watcher().onBreak();
		}
	}
}

}
