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
#include <vector>
#include <tarte/Handler.h>
#include <tarte/Platform.h>
#include <tarte/ClassUtil.h>
#include "SoundSpec.h"

namespace chisa {
using namespace tarte;

class Instrument;
class Quartet : public HandlerBody<Quartet> {
public:
	struct Player{
		DEFAULT_COPY_AND_ASSIGN(Player);
		Player() = delete;
		Player(Handler<Instrument> const& inst, std::size_t buflen);
		~Player() = default;
		Handler<Instrument> instrument;
		std::vector<unsigned char> buffer;
	};
	class Lock {
		STACK_OBJECT(Lock);
		DISABLE_COPY_AND_ASSIGN(Lock);
	private:
		Quartet& self_;
		inline Lock(Handler<Quartet> const& q) noexcept:self_(*q.get()) { self_.lock(); }
		inline ~Lock() noexcept { self_.unlock(); }
	};
private:
	std::vector<Player> instruments_;
	SoundSpec desiredSpec_;
	SoundSpec realSpec_;
public:
	Quartet(SoundSpec const& desired);
	virtual ~Quartet() noexcept = default;
	inline bool onFree() const noexcept { return false; };
private:
	void updateBufferSize();
public:
	inline unsigned int frequency() const noexcept { return this->realSpec_.frequency(); };
	inline unsigned int channels() const noexcept { return this->realSpec_.channels(); };
	inline unsigned int samples() const noexcept { return this->realSpec_.samples(); };
	inline SoundSpec::DataFormat format() const noexcept { return this->realSpec_.format(); };
public:
	bool start();
	bool stop();
	void onPlay(unsigned char* stream, int len);
	bool lock() noexcept;
	bool unlock() noexcept;
public:
	bool addInstrument(Handler<Instrument> const& inst);
	bool hasInstrument(Handler<Instrument> const& inst);
	bool removeInstrument(Handler<Instrument> const& inst);
protected:
	inline std::vector<Player> const& instruments() const noexcept { return this->instruments_; };
	void notifySoundSpec(SoundSpec::DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples);
	void notifySoundSpec(SoundSpec const& spec);
protected:
	virtual bool startImpl() = 0;
	virtual bool stopImpl() = 0;
	virtual void playImpl(Handler<Instrument> const& inst) = 0;
	virtual bool lockImpl() noexcept = 0;
	virtual bool unlockImpl() noexcept = 0;
};

}
