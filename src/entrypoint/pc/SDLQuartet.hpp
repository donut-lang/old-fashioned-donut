/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <SDL2/SDL.h>
#include <cinamo/Exception.h>
#include "../../chisa/audio/Quartet.hpp"

namespace chisa {

class SDLQuartet: public ::chisa::Quartet {
	static void __callback(void* userdata, Uint8* stream, int len);
public:
	SDLQuartet(const SoundSpec& desired);
	virtual ~SDLQuartet() noexcept;
private:
	virtual bool startImpl() override final;
	virtual bool stopImpl() override final;
	std::vector<char> mixBuffer_;
	virtual void playImpl(unsigned char* stream, const int len) override final;
	virtual bool lockImpl() noexcept override final;
	virtual bool unlockImpl() noexcept override final;
};



}
