#include <cstddef>
#include <stdio.h>
#include <fstream>
#include "exception/EmulatorException.h"
#include "VirtualMachine.h"

namespace nes {

VirtualMachine::VirtualMachine(VideoFairy& videoFairy, AudioFairy& audioFairy, GamepadFairy* player1, GamepadFairy* player2)
:ram(*this)
,processor(*this)
,audio(*this, audioFairy)
,video(*this, videoFairy)
,cartridge(NULL)
,ioPort(*this, player1, player2)
,clockDelta(0)
,resetFlag(false)
,hardResetFlag(false)
,irqLine(0)
{
}

VirtualMachine::~VirtualMachine()
{
	if(this->cartridge != NULL){
		delete this->cartridge;
	}
}
void VirtualMachine::run()
{
	if(this->hardResetFlag){
		this->clockDelta = 0;
		this->hardResetFlag = false;

		this->processor.onHardReset();

		this->cartridge->onHardReset();

		this->video.onHardReset();

		this->audio.onHardReset();

		return;
	}else if(this->resetFlag){
		this->clockDelta = 0;
		this->resetFlag = false;

		this->processor.onReset();

		this->cartridge->onReset();

		this->video.onReset();

		this->audio.onReset();

		return;
	}

	const int32_t cpuClockDelta = this->clockDelta / CPU_CLOCK_FACTOR;
	const int32_t videoClockDelta = this->clockDelta / VIDEO_CLOCK_FACTOR;
	this->clockDelta  = 0;

	this->processor.run(cpuClockDelta);

	this->video.run(videoClockDelta);

	this->cartridge->run(cpuClockDelta);

	this->audio.run(cpuClockDelta);

}

void VirtualMachine::consumeClock(uint32_t clock)
{
	this->clockDelta += clock;
}

void VirtualMachine::sendVBlank()
{
	this->ioPort.onVBlank();
}

void VirtualMachine::sendNMI()
{
	this->processor.sendNMI();
}

void VirtualMachine::reserveIRQ(uint8_t device)
{
	this->irqLine |= device;
	this->processor.reserveIRQ();
}

void VirtualMachine::releaseIRQ(uint8_t device)
{
	this->irqLine &= ~(device);
	if(irqLine == 0){
		this->processor.releaseIRQ();
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
	if(this->cartridge){
		delete this->cartridge;
	}
	this->cartridge = Cartridge::loadCartridge(*this, filename);
	this->video.connectCartridge(this->cartridge);
}

XValue VirtualMachine::save()
{
	Handler<XObject> xobj(new XObject);
	xobj->set("ram", (XArchiverOut() << this->ram).toXValue());
	xobj->set("processor", (XArchiverOut() << this->processor).toXValue());
	xobj->set("audio", (XArchiverOut() << this->audio).toXValue());
	xobj->set("video", (XArchiverOut() << this->video).toXValue());
	xobj->set("cartridge", this->cartridge->save());
	xobj->set("ioPort", (XArchiverOut() << this->ioPort).toXValue());

	xobj->set("clockDelta", this->clockDelta);
	xobj->set("resetFlag", this->resetFlag);
	xobj->set("hardResetFlag", this->hardResetFlag);
	xobj->set("irqLine", this->irqLine);
	return xobj;
}
void VirtualMachine::load(XValue const& data)
{
	if(this->cartridge){
		delete this->cartridge;
	}
	Handler<XObject> xobj(data.as<XObject>());
	XArchiverIn(xobj->get<XValue>("ram")) >> this->ram;
	XArchiverIn(xobj->get<XValue>("processor")) >> this->processor;
	XArchiverIn(xobj->get<XValue>("audio")) >> this->audio;
	XArchiverIn(xobj->get<XValue>("video")) >> this->video;
	this->cartridge = Cartridge::load(*this, xobj->get<XValue>("cartridge"));
	XArchiverIn(xobj->get<XValue>("ioPort")) >> this->ioPort;

	xobj->get("clockDelta", this->clockDelta);
	xobj->get("resetFlag", this->resetFlag);
	xobj->get("hardResetFlag", this->hardResetFlag);
	xobj->get("irqLine", this->irqLine);
	//最後にビデオの接続。これでOK？
	this->video.connectCartridge(this->cartridge);
}

}
