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

class SDLPlatformFairy: public PlatformFairy {
private:
	SDL_Window* window_;
	SDL_GLContext gl_;
public:
	SDLPlatformFairy(Logger& log);
	virtual ~SDLPlatformFairy() noexcept;
private:
	virtual void init(const std::string& title, int width, int height, bool isFullScreen, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits) override final;
	virtual void setTitle(const std::string& name);
	virtual void swapBuffer();
	virtual unsigned int getTimeMs();
	virtual void sleepMs(unsigned int ms);
	virtual bool pollEvent(Chisa& chisa);
	virtual void startIME(const geom::Area& area) override final;
	virtual void stopIME() override final;
	virtual Handler< ::chisa::Quartet> createQuartet() override final;
	virtual Handler< ::chisa::JoystickManager> createJoystickManager() override final;
};

}
