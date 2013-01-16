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

#include <iostream>
#include <SDL2/SDL.h>
#include <tarte/Exception.h>
#include <tarte/Logger.h>

#include "../../chisa/Chisa.h"
#include "../../chisa/audio/Quartet.h"
namespace chisa {
using namespace tarte;


class SDLQuartet : public ::chisa::Quartet {
	static void __callback(void *userdata, Uint8 *stream, int len){
		static_cast<SDLQuartet*>(userdata)->onPlay(stream, len);
	}
public:
	SDLQuartet(SoundSpec const& desired)
	:Quartet(desired){
		SDL_AudioSpec spec;
		spec.channels = desired.channels();
		spec.format = desired.format();
		spec.freq = desired.frequency();
		spec.samples = desired.samples();
		spec.callback = __callback;
		spec.userdata = this;
		SDL_AudioSpec obtained;
		if(SDL_OpenAudio(&spec, &obtained) != 0){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to open audio. SDL says: \"%s\"", SDL_GetError());
		}
		this->notifySoundSpec(SoundSpec::DataFormat(obtained.format), obtained.channels, obtained.freq, obtained.samples);
	}
	virtual ~SDLQuartet() noexcept
	{
		Lock lock(*this);
		SDL_PauseAudio(1);
		SDL_CloseAudio();
	}
private:
	virtual bool startImpl() override final
	{
		SDL_PauseAudio(0);
		return true;
	}
	virtual bool stopImpl() override final
	{
		SDL_PauseAudio(1);
		return true;
	}
	std::vector<char> mixBuffer_;
	virtual void playImpl(unsigned char* stream, int const len) override final
	{
		if(this->players().size() == 1) { //最適化：ミックスしなくていい
			Player& first = this->players().front();
			SoundSpec const& firstSpec = first.instrument->spec();
			if(this->spec().isCompatibleTo(firstSpec)){ //最適化：変換もしなくていい
				std::copy(first.buffer.begin(), first.buffer.end(), stream);
			}else{ //変換必要
				SDL_AudioCVT cvt;
				int const result = SDL_BuildAudioCVT(&cvt,
						SDL_AudioFormat(firstSpec.format()),
						firstSpec.channels(),
						firstSpec.frequency(),
						SDL_AudioFormat(this->spec().format()),
						this->spec().channels(),
						this->spec().frequency()
						);
				if(result == 0){
					TARTE_EXCEPTION(Exception, "[BUG] Oops. It is not need to convert.")
				}else if(result < 0){
					TARTE_EXCEPTION(Exception, "[BUG] Oops. This format is not compatible. SDL says: \"%s\"", SDL_GetError())
				}
				const unsigned int temporaryBufferSize = first.buffer.size() * cvt.len_mult;
				bool needCopy = true;
				if(temporaryBufferSize <= static_cast<unsigned int>(len)){
					std::copy(first.buffer.begin(), first.buffer.end(), stream);
					cvt.buf = stream;
					cvt.len = first.buffer.size();
					needCopy = false;
				}else if(temporaryBufferSize <= first.buffer.size()){
					cvt.buf = first.buffer.data();
					cvt.len = first.buffer.size();
				}else{
					this->mixBuffer_.resize(temporaryBufferSize);
					std::copy(first.buffer.begin(), first.buffer.end(), this->mixBuffer_.begin());
					cvt.buf = reinterpret_cast<unsigned char*>(this->mixBuffer_.data());
					cvt.len = first.buffer.size();
				}
				if(SDL_ConvertAudio(&cvt) != 0){
					TARTE_EXCEPTION(Exception, "[BUG] Oops. Failed to convert audio. SDL says: \"%s\"", SDL_GetError())
				}else if (cvt.len_cvt != len) {
					TARTE_EXCEPTION(Exception, "[BUG] Oops. Buffer size does not match! converted: %d != stream: %d", cvt.len_cvt, stream);
				}
				if(needCopy){
					std::memcpy(stream, cvt.buf, cvt.len_cvt);
				}
			}
			return;
		}
		/**************************************************
		 * 複数チャンネル
		 **************************************************/
		std::memset(stream, 0, len);
		unsigned int const volume = 128/this->players().size();
		for(Player& player : this->players()){
			SoundSpec const& spec(player.instrument->spec());
			if(spec.isCompatibleTo(this->spec())) { //変換必要なし
				SDL_MixAudio(stream, player.buffer.data(), len, volume);
				continue;
			}
			SDL_AudioCVT cvt;
			int const result = SDL_BuildAudioCVT(&cvt,
					SDL_AudioFormat(spec.format()),
					spec.channels(),
					spec.frequency(),
					SDL_AudioFormat(this->spec().format()),
					this->spec().channels(),
					this->spec().frequency()
					);
			if(result == 0){
				TARTE_EXCEPTION(Exception, "[BUG] Oops. It is not need to convert.")
			}else if(result < 0){
				TARTE_EXCEPTION(Exception, "[BUG] Oops. This format is not compatible. SDL says: \"%s\"", SDL_GetError())
			}
			const unsigned int temporaryBufferSize = player.buffer.size() * cvt.len_mult;
			if(temporaryBufferSize <= player.buffer.size()){
				cvt.buf = player.buffer.data();
				cvt.len = player.buffer.size();
			} else {
				this->mixBuffer_.resize(temporaryBufferSize);
				std::copy(player.buffer.begin(), player.buffer.end(), this->mixBuffer_.begin());
				cvt.buf = reinterpret_cast<unsigned char*>(this->mixBuffer_.data());
				cvt.len = player.buffer.size();
			}
			if(SDL_ConvertAudio(&cvt) != 0){
				TARTE_EXCEPTION(Exception, "[BUG] Oops. Failed to convert audio. SDL says: \"%s\"", SDL_GetError())
			}else if (cvt.len_cvt != len) {
				TARTE_EXCEPTION(Exception, "[BUG] Oops. Buffer size does not match! converted: %d != stream: %d", cvt.len_cvt, stream);
			}
			SDL_MixAudio(stream, cvt.buf, cvt.len_cvt, volume);
		}
	}
	virtual bool lockImpl() noexcept override final
	{
		SDL_LockAudio();
		return true;
	}
	virtual bool unlockImpl() noexcept override final
	{
		SDL_UnlockAudio();
		return true;
	}
};

class SDLJoystick : public Joystick {
private:
	SDL_Joystick* joy_;
public:
	SDLJoystick(Handler<JoystickManager> manager, SDL_Joystick* joy)
	:Joystick(manager)
	,joy_(joy)
	{

	}
	virtual ~SDLJoystick() noexcept {
		SDL_JoystickClose(joy_);
	}
protected:
	virtual unsigned int numAxesImpl() noexcept {
		return SDL_JoystickNumAxes(joy_);
	}
	virtual unsigned int numBallsImpl() noexcept {
		return SDL_JoystickNumBalls(joy_);
	}
	virtual unsigned int numButtonsImpl() noexcept {
		return SDL_JoystickNumButtons(joy_);
	}
protected:
	virtual int axisImpl(unsigned int const& idx) noexcept {
		return SDL_JoystickGetAxis(joy_, idx);
	}
	virtual void ballImpl(unsigned int const& idx, int& dx, int& dy) {
		SDL_JoystickGetBall(joy_, idx, &dx, &dy);
	}
	virtual bool buttonImpl(unsigned int const& idx) noexcept {
		return SDL_JoystickGetButton(joy_, idx) != 0;
	}
protected:
	virtual void updateImpl() noexcept {
		SDL_JoystickUpdate();
	}
};

class SDLJoystickManager : public JoystickManager {
public:
	SDLJoystickManager(){}
	virtual ~SDLJoystickManager() noexcept = default;
private:
	virtual unsigned int numJoysticksImpl() noexcept {
		return SDL_NumJoysticks();
	}
	virtual Handler<Joystick> joystickImpl( unsigned int id ) {
		Handler<Joystick> joy(new SDLJoystick(self(), SDL_JoystickOpen(id)));
		return joy;
	}
};

class SDLPlatformFairy : public PlatformFairy {
private:
	SDL_Window* window_;
	SDL_GLContext gl_;
public:
	SDLPlatformFairy(Logger& log)
	:PlatformFairy(log)
	,window_(nullptr)
	,gl_(nullptr)
	{
		if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0 ){
			TARTE_EXCEPTION(Exception, "[BUG] Failed to init SDL2");
		}
	}
	virtual ~SDLPlatformFairy() noexcept {
		SDL_GL_DeleteContext(gl_);
		SDL_DestroyWindow(this->window_);
		SDL_Quit();
	}
private:
	virtual void init(std::string const& title, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits ) {
		this->window_ = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);
		if (!this->window_) {
			TARTE_EXCEPTION(Exception, SDL_GetError());
		}
		this->gl_ = SDL_GL_CreateContext(this->window_);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, redbits);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, greenbits);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, bluebits);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alphabits);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthbits);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilbits);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}
	virtual void setTitle(std::string const& name) {
		SDL_SetWindowTitle(this->window_, name.c_str());
	}
	virtual void swapBuffer()
	{
		SDL_GL_SwapWindow(this->window_);
	}
	virtual unsigned int getTimeMs()
	{
		return SDL_GetTicks();
	}
	virtual void sleepMs(unsigned int ms)
	{
		SDL_Delay(ms);
	}
	virtual bool pollEvent(Chisa& chisa)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev) == SDL_TRUE){
			switch(ev.type){
			case SDL_WINDOWEVENT: {
				if(ev.window.event == SDL_WINDOWEVENT_RESIZED) {
					chisa.reshape(ev.window.data1, ev.window.data2);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
				if(ev.button.button == SDL_BUTTON_LEFT) {
					chisa.pointerDown(ev.button.timestamp, 0, geom::Point(ev.button.x, ev.button.y));
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(ev.button.button == SDL_BUTTON_LEFT) {
					chisa.pointerUp(ev.button.timestamp, 0, geom::Point(ev.button.x, ev.button.y));
				}
				break;
			case SDL_MOUSEMOTION:
				if(ev.motion.state & SDL_BUTTON_LMASK) {
					chisa.pointerMove(ev.motion.timestamp, 0, geom::Point(ev.motion.x, ev.motion.y));
				}
				break;
			case SDL_MOUSEWHEEL:
				chisa.pointerScroll(ev.wheel.timestamp, ev.wheel.y*50);
				break;
			case SDL_QUIT:
				return false;
			case SDL_TEXTINPUT:
				chisa.textInput(ev.text.timestamp, ev.text.text);
				break;
			case SDL_TEXTEDITING:
				chisa.textEdit(ev.edit.timestamp, ev.edit.text, ev.edit.start, ev.edit.length);
				break;
			case SDL_KEYDOWN:
				chisa.keyDown(ev.key.timestamp, ev.key.repeat, ev.key.keysym);
				break;
			case SDL_KEYUP:
				chisa.keyUp(ev.key.timestamp, ev.key.keysym);
				break;
			default:
				break;
			}
		}
		return true;
	}
	virtual void startIME(geom::Area const& area) override final
	{
		SDL_StartTextInput();
		SDL_Rect rect;
		rect.x = area.x();
		rect.y = area.y();
		rect.w = area.width();
		rect.h = area.height();
		SDL_SetTextInputRect(&rect);
	}
	virtual void stopIME() override final
	{
		SDL_StopTextInput();
	}
	virtual Handler< ::chisa::Quartet> createQuartet() override final
	{
		return Handler< ::chisa::Quartet>( new SDLQuartet(SoundSpec(SoundSpec::DataFormat::S16SYS, 1, 44100, 8192)) );
	}
	virtual Handler< ::chisa::JoystickManager> createJoystickManager() override final {
		return Handler< ::chisa::JoystickManager> ( new SDLJoystickManager() );
	}
};

}
