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
#include <cinamo/Handler.h>
#include "SoundSpec.h"

namespace chisa {
using namespace cinamo;

class Quartet;
class Instrument : public HandlerBody<Instrument> {
private:
	HandlerW<Quartet> quartet_;
	SoundSpec spec_;
public:
	Instrument();
	virtual ~Instrument() noexcept = default;
	inline bool onFree() const noexcept { return false; };
public:
	Handler<Quartet> quartet() const;
	SoundSpec onConnected(Handler<Quartet> const& quartet, SoundSpec const& spec);
	void onDisconnected();
	SoundSpec spec() noexcept;
	void play(unsigned char *stream, int len);
protected:
	virtual SoundSpec querySpec(SoundSpec const& spec) noexcept = 0;
	virtual void playImpl(unsigned char *stream, int len) = 0;
};

}
