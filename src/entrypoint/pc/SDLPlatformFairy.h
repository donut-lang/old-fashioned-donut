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
namespace chisa {
using namespace tarte;

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
		if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 ){
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
				break;
			case SDL_TEXTEDITING:
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
	virtual void stopIME()
	{
		SDL_StopTextInput();
	}
};

}
