#include <cstddef>
#include <stdio.h>
#include <fstream>
#include "exception/EmulatorException.h"
#include "VirtualMachine.h"

namespace nes {

VirtualMachine::VirtualMachine(DebuggerFairy& debuggerFairy, VideoFairy& videoFairy, AudioFairy& audioFairy, GamepadFairy* player1, GamepadFairy* player2)
:debugger_(*this, debuggerFairy)
,ram_(*this)
,processor_(*this)
,audio_(*this, audioFairy)
,video_(*this, videoFairy)
,cartridge_(NULL)
,ioPort_(*this, player1, player2)
,clockDelta(0)
,resetFlag(false)
,hardResetFlag(false)
,irqLine(0)
{
}

VirtualMachine::~VirtualMachine()
{
	if(this->cartridge_ != NULL){
		delete this->cartridge_;
	}
}
void VirtualMachine::run()
{
	this->debugger_.processorExecute(this->processor_.getPC());
	if(this->hardResetFlag){
		this->clockDelta = 0;
		this->hardResetFlag = false;

		this->processor_.onHardReset();

		this->cartridge_->onHardReset();

		this->video_.onHardReset();

		this->audio_.onHardReset();

		return;
	}else if(this->resetFlag){
		this->clockDelta = 0;
		this->resetFlag = false;

		this->processor_.onReset();

		this->cartridge_->onReset();

		this->video_.onReset();

		this->audio_.onReset();

		return;
	}

	const int32_t cpuClockDelta = this->clockDelta / CPU_CLOCK_FACTOR;
	const int32_t videoClockDelta = this->clockDelta / VIDEO_CLOCK_FACTOR;
	this->clockDelta  = 0;

	this->processor_.run(cpuClockDelta);

	this->video_.run(videoClockDelta);

	this->cartridge_->run(cpuClockDelta);

	this->audio_.run(cpuClockDelta);

}

void VirtualMachine::consumeClock(uint32_t clock)
{
	this->clockDelta += clock;
}

void VirtualMachine::sendVBlank()
{
	this->ioPort_.onVBlank();
}

void VirtualMachine::sendNMI()
{
	this->processor_.sendNMI();
}

void VirtualMachine::reserveIRQ(uint8_t device)
{
	this->irqLine |= device;
	this->processor_.reserveIRQ();
}

void VirtualMachine::releaseIRQ(uint8_t device)
{
	this->irqLine &= ~(device);
	if(irqLine == 0){
		this->processor_.releaseIRQ();
	}
}

bool VirtualMachine::isIRQpending(uint8_t device)
{
	return (irqLine & device) == device;
}

void VirtualMachine::sendHardReset()
{
	this->hardResetFlag = true;
}
void VirtualMachine::sendReset()
{
	this->resetFlag = true;
}

void VirtualMachine::loadCartridge(const char* filename)
{
	if(this->cartridge_){
		delete this->cartridge_;
	}
	this->cartridge_ = Cartridge::loadCartridge(*this, filename);
	this->video_.connectCartridge(this->cartridge_);
}

XValue VirtualMachine::save()
{
	Handler<XObject> xobj(new XObject);
	xobj->set("ram", (XArchiverOut() << this->ram_).toXValue());
	xobj->set("processor", (XArchiverOut() << this->processor_).toXValue());
	xobj->set("audio", (XArchiverOut() << this->audio_).toXValue());
	xobj->set("video", (XArchiverOut() << this->video_).toXValue());
	xobj->set("cartridge", this->cartridge_->save());
	xobj->set("ioPort", (XArchiverOut() << this->ioPort_).toXValue());

	xobj->set("clockDelta", this->clockDelta);
	xobj->set("resetFlag", this->resetFlag);
	xobj->set("hardResetFlag", this->hardResetFlag);
	xobj->set("irqLine", this->irqLine);

	xobj->set("debugger", (XArchiverOut() << this->debugger_).toXValue());
	return xobj;
}
void VirtualMachine::load(XValue const& data)
{
	if(this->cartridge_){
		delete this->cartridge_;
	}
	Handler<XObject> xobj(data.as<XObject>());
	XArchiverIn(xobj->get<XValue>("ram")) >> this->ram_;
	XArchiverIn(xobj->get<XValue>("processor")) >> this->processor_;
	XArchiverIn(xobj->get<XValue>("audio")) >> this->audio_;
	XArchiverIn(xobj->get<XValue>("video")) >> this->video_;
	this->cartridge_ = Cartridge::load(*this, xobj->get<XValue>("cartridge"));
	XArchiverIn(xobj->get<XValue>("ioPort")) >> this->ioPort_;

	xobj->get("clockDelta", this->clockDelta);
	xobj->get("resetFlag", this->resetFlag);
	xobj->get("hardResetFlag", this->hardResetFlag);
	xobj->get("irqLine", this->irqLine);
	XArchiverIn(xobj->get<XValue>("debugger")) >> this->debugger_;
	//最後にビデオの接続。これでOK？
	this->video_.connectCartridge(this->cartridge_);
}

}
