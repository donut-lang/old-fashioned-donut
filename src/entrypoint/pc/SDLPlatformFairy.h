/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <cinamo/Exception.h>
#include <cinamo/Logger.h>

#include "../../chisa/Chisa.h"
#include "../../chisa/audio/Quartet.h"
namespace chisa {
using namespace cinamo;

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
