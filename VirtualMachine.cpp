#include <cstddef>
#include <stdio.h>
#include "exception/EmulatorException.h"
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine(VideoFairy& videoFairy, AudioFairy& audioFairy, GamepadFairy* player1, GamepadFairy* player2) :
ram(*this),
processor(*this),
audio(*this, audioFairy),
video(*this, videoFairy),
cartridge(NULL),
ioPort(*this, player1, player2),
resetFlag(false),
hardResetFlag(false)
{
    //ctor
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

    int32_t cpuClockDelta = this->clockDelta / CPU_CLOCK_FACTOR;
    int32_t videoClockDelta = this->clockDelta / VIDEO_CLOCK_FACTOR;
    this->clockDelta  = 0;

    this->processor.run(cpuClockDelta);

    this->cartridge->run(cpuClockDelta);

    this->video.run(videoClockDelta);

    this->audio.run(cpuClockDelta);

}

void VirtualMachine::consumeClock(uint32_t clock)
{
    this->clockDelta += clock;
}

void VirtualMachine::sendHBlank(uint16_t scanline)
{
    this->cartridge->onHBlank(scanline);
}
void VirtualMachine::sendVBlank()
{
	this->cartridge->onVBlank();
	this->ioPort.onVBlank();
}

void VirtualMachine::sendNMI()
{
    this->processor.sendNMI();
}

void VirtualMachine::sendIRQ()
{
    this->processor.sendIRQ();
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
