/**
 * Chisa
 * Copyright (C) 2013 psi
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

#include "SDLPlatformFairy.h"
#include "SDLJoystickManager.h"
#include "SDLQuartet.h"

namespace chisa {

SDLPlatformFairy::SDLPlatformFairy(Logger& log)
:PlatformFairy(log)
,window_(nullptr)
,gl_(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
		TARTE_EXCEPTION(Exception, "[BUG] Failed to init SDL2");
	}
}

SDLPlatformFairy::~SDLPlatformFairy() noexcept
{
	SDL_LockAudio();
	this->log().d("SDL", "Pausing audio...");
	SDL_PauseAudio(1);
	this->log().d("SDL", "paused.");
	SDL_UnlockAudio();
	this->log().d("SDL", "Closing audio...");
	SDL_CloseAudio();
	this->log().d("SDL", "Closed.");
	this->log().d("SDL", "Deleting GL context...");
	SDL_GL_DeleteContext(gl_);
	this->log().d("SDL", "Context deleted.");
	SDL_DestroyWindow(this->window_);
	this->log().d("SDL", "Window destroyed.");
	SDL_Quit();
	this->log().d("SDL", "Quit.");
}

void SDLPlatformFairy::init(const std::string& title, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits)
{
	this->window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
	//XXX: ワークアラウンド。SDL2がリサイズ出来るようにした時にうまく対処できない
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // | SDL_WINDOW_RESIZABLE
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

void SDLPlatformFairy::setTitle(const std::string& name)
{
	SDL_SetWindowTitle(this->window_, name.c_str());
}

void SDLPlatformFairy::swapBuffer()
{
	SDL_GL_SwapWindow(this->window_);
}

unsigned int SDLPlatformFairy::getTimeMs()
{
	return SDL_GetTicks();
}

void SDLPlatformFairy::sleepMs(unsigned int ms)
{
	SDL_Delay(ms);
}

bool SDLPlatformFairy::pollEvent(Chisa& chisa)
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev) == SDL_TRUE) {
		switch (ev.type) {
		case SDL_WINDOWEVENT: {
			switch (ev.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				chisa.reshape(ev.window.data1, ev.window.data2);
				break;
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_EXPOSED:
				chisa.onShown();
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_HIDDEN:
				chisa.onHidden();
				break;
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.button == SDL_BUTTON_LEFT) {
				chisa.pointerDown(ev.button.timestamp, 0, geom::Point(ev.button.x, ev.button.y));
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (ev.button.button == SDL_BUTTON_LEFT) {
				chisa.pointerUp(ev.button.timestamp, 0, geom::Point(ev.button.x, ev.button.y));
			}
			break;
		case SDL_MOUSEMOTION:
			if (ev.motion.state & SDL_BUTTON_LMASK) {
				chisa.pointerMove(ev.motion.timestamp, 0, geom::Point(ev.motion.x, ev.motion.y));
			}
			break;
		case SDL_MOUSEWHEEL:
			chisa.pointerScroll(ev.wheel.timestamp, ev.wheel.y);
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

void SDLPlatformFairy::startIME(const geom::Area& area)
{
	SDL_StartTextInput();
	SDL_Rect rect;
	rect.x = area.x();
	rect.y = area.y();
	rect.w = area.width();
	rect.h = area.height();
	SDL_SetTextInputRect(&rect);
}

void SDLPlatformFairy::stopIME()
{
	SDL_StopTextInput();
}

Handler< ::chisa::Quartet> SDLPlatformFairy::createQuartet()
{
	return Handler< ::chisa::Quartet>(new SDLQuartet(SoundSpec(SoundSpec::DataFormat::S16SYS, 1, 44100, 8192)));
}

Handler< ::chisa::JoystickManager> SDLPlatformFairy::createJoystickManager()
{
	return Handler< ::chisa::JoystickManager>(new SDLJoystickManager());
}

}
