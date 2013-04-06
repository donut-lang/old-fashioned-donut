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
#include <cinamo/Handler.h>
#include "PlatformFairy.h"
#include "tk/Universe.h"
#include "Hexe.h"

namespace chisa {
using namespace cinamo;

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
private:
	bool isHidden_;
public:
	void init(std::string const& windowTitle, int width, int height, bool isFullScreen, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits );
	void setTitle(std::string const& name);
	void start(std::string const& initialWorldName);
private:
	void loop();
public:
	void onMouseBack(float const& timeMs);
	void onMouseForward(float const& timeMs);
	void onShown();
	void onHidden();
	void reshape(float const& width, float const& height);
	void pointerDown(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerUp(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerMove(float const& timeMs, unsigned int const& pointerIndex, geom::Point const& screenPoint);
	void pointerScroll(float const& timeMs, float const& ratio);
	void textInput(float const& timeMs, std::string const& text);
	void textEdit(float const& timeMs, std::string const& text, int const& start, int const& length);
	void keyDown(float const& timeMs, const bool isRepeat, SDL_Keysym const& sym);
	void keyUp(float const& timeMs, SDL_Keysym const& sym);
};

}
