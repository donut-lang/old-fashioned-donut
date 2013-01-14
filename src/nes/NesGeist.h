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
#include "machine/VirtualMachine.h"
#include "../chisa/Hexe.h"
#include "../chisa/tk/Task.h"
#include "../chisa/gl/Sprite.h"
#include "../chisa/audio/Instrument.h"
#include "../chisa/input/Joystick.h"
#include <tarte/Thread.h>
#include <tarte/Logger.h>

namespace nes {
using namespace tarte;

class Hexe;

class NesGeist : public chisa::WorldGeist {
private:
	class Runner {
	private:
		NesGeist& parent_;
		bool stop_;
	public:
		Runner(NesGeist& parent);
		virtual ~Runner() = default;
	public:
		void queryStop();
	public:
		void operator()();
	};
private:
	class Video final : public VideoFairy {
	private:
		NesGeist& self_;
	public:
		Video(NesGeist& self);
		virtual void dispatchRendering(const uint8_t (&nesBuffer)[screenHeight][screenWidth], const uint8_t paletteMask) override final;
	};
	class Audio final : public AudioFairy {
	private:
		NesGeist& self_;
		::tarte::Handler<chisa::Instrument> inst_;
	public:
		Audio(NesGeist& self);
		::tarte::Handler<chisa::Instrument> instrument();
	public:
		class Instrument : public chisa::Instrument {
		private:
			Audio& self_;
		public:
			Instrument(Audio& self);
			virtual ~Instrument() noexcept = default;
		private:
			virtual ::chisa::SoundSpec querySpec(::chisa::SoundSpec const& spec) noexcept override final;
			virtual void playImpl(unsigned char *stream, int len) override final;
		};
	};
	class Gamepad final : public GamepadFairy {
	private:
		unsigned char joyState_;
		Handler<chisa::Joystick> joystick_;
	private:
		virtual void onUpdate() override;
		virtual bool isPressed(uint8_t keyIdx) override;
	public:
		Gamepad(Handler<chisa::JoystickManager> const& mgr);
		void updateJoystick();
	};
public:
	Handler<Hexe> hexe();
public:
	class Lock {
		NesGeist& parent_;
	public:
		Lock(NesGeist& parent);
		virtual ~Lock();
		inline Handler<chisa::gl::Sprite> getSprite() { return parent_.spr_; };
	};
private:
	VirtualMachine* machine_;
	std::thread* runner_t_;
	Runner* runner_;
	Handler<chisa::gl::Sprite> spr_;
	std::mutex spr_mutex_;
	std::mutex frame_mutex_;
	float time_ms_;
	std::condition_variable cond_;
	Video video_;
	Audio audio_;
	Gamepad gamepad_;
public:
	NesGeist(Logger& log, ::tarte::Handler<Hexe> const& hexe, ::tarte::HandlerW<chisa::tk::World> world);
	virtual ~NesGeist() noexcept;
	virtual std::string toString() const override final;
	inline VirtualMachine* machine() const noexcept { return this->machine_; };
private:
	virtual Handler< ::donut::Object> createDonutObject(Handler< ::donut::Heap> const& heap) override final;
public:
	void stopNES();
	void loadNES(std::string const& abs_filename);
	void startNES();
public:
	XValue saveNES();
	void loadNES(XValue const& data);
};

}
