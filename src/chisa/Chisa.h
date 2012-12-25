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

#pragma once
#include <tarte/Handler.h>
#include "tk/Universe.h"
#include "Hexe.h"

namespace chisa {
using namespace tarte;

class Chisa;

class PlatformFairy : public HandlerBody<PlatformFairy> {
	DEFINE_MEMBER_REF(protected, Logger, log);
protected:
	PlatformFairy(Logger& log):log_(log){};
	virtual ~PlatformFairy() noexcept = default;
public:
	inline bool onFree() noexcept { return false; };
public:
	virtual void init(Chisa& chisa, std::string const& windowTitle, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits ) = 0;
	virtual void setTitle(Chisa& chisa, std::string const& name) = 0;
	virtual void swapBuffer(Chisa& chisa) = 0;
	virtual unsigned int getTimeMs(Chisa& chisa) = 0;
	virtual void sleepMs(Chisa& chisa, unsigned int ms) = 0;
	virtual bool pollEvent(Chisa& chisa) = 0;
	virtual void startIME(Chisa& chisa) = 0;
	virtual void exitIME(Chisa& chisa) = 0;
};

class Chisa final : public HandlerBody<Chisa> {
	DEFINE_MEMBER_REF(protected, Logger, log)
public:
	Chisa(Logger& log, Handler<PlatformFairy> fairy, Handler<Hexe> hexe);
	~Chisa() noexcept = default;
	inline bool onFree() noexcept { return false; };
private:
	Handler<PlatformFairy> fairy_;
	Handler<Hexe> hexe_;
	Handler<tk::Universe> universe_;
public:
	void init(std::string const& windowTitle, int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits );
	void setTitle(std::string const& name);
	void start(std::string const& initialWorldName);
private:
	void loop();
public:
	void reshape(float const& width, float const& height);
	void pointerDown(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerUp(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerMove(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerScroll(float const& timeMs, float const& ratio);
};

}
