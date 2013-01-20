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

#pragma once
#include <SDL2/SDL.h>
#include <tarte/Exception.h>
#include "../../chisa/audio/Quartet.h"

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
