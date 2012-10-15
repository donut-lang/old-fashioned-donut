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

#ifndef _CXX_Chisa_NESGEIST_H_
#define _CXX_Chisa_NESGEIST_H_

#include "machine/VirtualMachine.h"
#include "../chisa/Hexe.h"
#include "../chisa/gl/Sprite.h"
#include "../chisa/gl/RawSprite.h"
#include "../chisa/util/Thread.h"

namespace nes {

class NesGeist : public chisa::WorldGeist, public VideoFairy, public AudioFairy, public GamepadFairy, public chisa::util::Thread {
	VirtualMachine* machine_;
	chisa::gl::Handler<chisa::gl::RawSprite> sprA_;
	chisa::util::RWLock sprLock_;
public:
	NesGeist(chisa::logging::Logger& log, std::weak_ptr<chisa::tk::World> world);
	virtual ~NesGeist();
	virtual std::string toString() const override;
public:
	virtual void dispatchRendering(const uint8_t nesBuffer[screenHeight][screenWidth], const uint8_t paletteMask) override;
	virtual void onUpdate() override;
	virtual bool isPressed(uint8_t keyIdx) override;
	virtual void run() override;
public:
	void loadNES(const std::string& abs_filename);
	void startNES();
};

}
#endif /* INCLUDE_GUARD */
