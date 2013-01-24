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
#include <SDL2/SDL.h>

namespace nes {

static std::string const TAG("NesFairy");

/**********************************************************************************************************************
 * Video
 **********************************************************************************************************************/

NesGeist::Video::Video(NesGeist& geist)
:self_(geist)
{

}

void NesGeist::Video::dispatchRendering(const uint8_t (&nesBuffer)[screenHeight][screenWidth], const uint8_t paletteMask)
{
	using namespace chisa::gl;
	{
		NesGeist::VideoLock lock(self_);
		Sprite::Session s(lock.getSprite());
		unsigned char* mem8 = s.data();
		unsigned int* mem32 = nullptr;
		const int stride = s.stride();
		for(int y=0;y<screenHeight;++y){
			mem32 = reinterpret_cast<unsigned int*>(mem8);
			for(int x=0;x<screenWidth;++x){
				mem32[x] = nesPaletteARGB[nesBuffer[y][x] & paletteMask];
			}
			mem8+=stride;
		}
	}
	{
		MachineUnlock unl(self_);
		std::unique_lock<std::mutex> lock(self_.cond_mutex_);
		if(self_.runner_t_){
			self_.cond_.wait_for(lock, std::chrono::milliseconds(20));
		}
	}
}

/**********************************************************************************************************************
 * Gamepad
 **********************************************************************************************************************/

NesGeist::Gamepad::Gamepad(NesGeist& self, Handler<chisa::JoystickManager> const& mgr)
:self_(self)
,joyState_(0)
,keyState_(0)
{
	//XXX: 設定画面、作りましょう
	if(mgr->numJoysticks() > 0) {
		for( std::size_t i = 0;i< mgr->numJoysticks(); ++i) {
			auto joy = mgr->joystick(i);
			if(joy->numButtons() >= 4){
				this->joystick_ = joy;
				if(self.log().d()){
					self.log().d(TAG, "Joystick found: %d %d buttons.", i, this->joystick_->numButtons());
				}
				break;
			}
		}
	}
	if( !this->joystick_ && self_.log().d() ) {
		self.log().d(TAG, "Joystick not found");
	}
}

void NesGeist::Gamepad::onUpdate()
{

}

void NesGeist::Gamepad::updateJoystick() {
	if(!this->joystick_) {
		return;
	}
	this->joystick_->update();
	this->joyState_ = 0;
	const int32_t x = this->joystick_->axis(0);
	const int32_t y = this->joystick_->axis(1);

	if(x > 16384){
		joyState_ |= GamepadFairy::MASK_RIGHT;
	}else if(x < -16384){
		joyState_ |= GamepadFairy::MASK_LEFT;
	}

	if(y > 16384){
		joyState_ |= GamepadFairy::MASK_DOWN;
	}else if(y < -16384){
		joyState_ |= GamepadFairy::MASK_UP;
	}

	joyState_ |= this->joystick_->button(0) ? GamepadFairy::MASK_A : 0;
	joyState_ |= this->joystick_->button(1) ? GamepadFairy::MASK_B : 0;
	joyState_ |= this->joystick_->button(7) ? GamepadFairy::MASK_START : 0;
	joyState_ |= this->joystick_->button(6) ? GamepadFairy::MASK_SELECT : 0;
}

bool NesGeist::Gamepad::isPressed(uint8_t keyIdx)
{
	return ((joyState_ | keyState_) >> keyIdx) & 1;
}

void NesGeist::Gamepad::onFocusGained()
{
	this->keyState_ = 0;
}

void NesGeist::Gamepad::onFocusLost()
{
	this->keyState_ = 0;
}

bool NesGeist::Gamepad::onKeyDown(bool isRepeat, const SDL_Keysym& sym)
{
	switch(sym.scancode){
	case SDL_SCANCODE_UP:
		keyState_ |= GamepadFairy::MASK_UP;
		break;
	case SDL_SCANCODE_DOWN:
		keyState_ |= GamepadFairy::MASK_DOWN;
		break;
	case SDL_SCANCODE_LEFT:
		keyState_ |= GamepadFairy::MASK_LEFT;
		break;
	case SDL_SCANCODE_RIGHT:
		keyState_ |= GamepadFairy::MASK_RIGHT;
		break;
	case SDL_SCANCODE_Z:
		keyState_ |= GamepadFairy::MASK_B;
		break;
	case SDL_SCANCODE_X:
		keyState_ |= GamepadFairy::MASK_A;
		break;
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_RETURN2:
		keyState_ |= GamepadFairy::MASK_START;
		break;
	case SDL_SCANCODE_BACKSPACE:
		keyState_ |= GamepadFairy::MASK_SELECT;
		break;
	default:
		break;
	}
	return true;
}

bool NesGeist::Gamepad::onKeyUp(const SDL_Keysym& sym)
{
	switch(sym.scancode){
	case SDL_SCANCODE_UP:
		keyState_ &= ~GamepadFairy::MASK_UP;
		break;
	case SDL_SCANCODE_DOWN:
		keyState_ &= ~GamepadFairy::MASK_DOWN;
		break;
	case SDL_SCANCODE_LEFT:
		keyState_ &= ~GamepadFairy::MASK_LEFT;
		break;
	case SDL_SCANCODE_RIGHT:
		keyState_ &= ~GamepadFairy::MASK_RIGHT;
		break;
	case SDL_SCANCODE_Z:
		keyState_ &= ~GamepadFairy::MASK_B;
		break;
	case SDL_SCANCODE_X:
		keyState_ &= ~GamepadFairy::MASK_A;
		break;
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_RETURN2:
		keyState_ &= ~GamepadFairy::MASK_START;
		break;
	case SDL_SCANCODE_BACKSPACE:
		keyState_ &= ~GamepadFairy::MASK_SELECT;
		break;
	default:
		break;
	}
	return true;
}
/**********************************************************************************************************************
 * Sound/Audio
 **********************************************************************************************************************/

NesGeist::Audio::Audio(NesGeist& geist)
:self_(geist){

}

NesGeist::Audio::Instrument::Instrument(Audio& self)
:self_(self)
{

}

::tarte::Handler<chisa::Instrument> NesGeist::Audio::instrument()
{
	if(this->inst_) {
		return this->inst_;
	}
	return (this->inst_ = ::tarte::Handler<chisa::Instrument>(new Instrument(*this)));
}

::chisa::SoundSpec NesGeist::Audio::Instrument::querySpec(::chisa::SoundSpec const& spec) noexcept
{
	return ::chisa::SoundSpec(::chisa::SoundSpec::DataFormat::S16SYS, 1, 44100, 8192);
}

void NesGeist::Audio::Instrument::playImpl(unsigned char *stream, int len)
{
	int written = (self_.popAudio(reinterpret_cast<int16_t*>(stream), len/sizeof(int16_t)))*sizeof(int16_t);
	std::memset(&stream[written], 0, len-written);
}


}

