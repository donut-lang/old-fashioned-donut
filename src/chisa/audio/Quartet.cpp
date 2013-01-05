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

#include <algorithm>
#include <tarte/Exception.h>
#include <cstring>
#include "Quartet.h"
#include "Instrument.h"

namespace chisa {

Quartet::Player::Player(Handler<Instrument> const& inst, std::size_t buflen)
:instrument(inst), buffer(buflen)
{

}

Quartet::Quartet(SoundSpec const& desired)
:desiredSpec_(desired), startRequested_(false)
{
}

void Quartet::notifySoundSpec(SoundSpec::DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples)
{
	this->notifySoundSpec(SoundSpec(format, channels, frequency, samples));
}

void Quartet::updateBufferSize()
{
	int num = this->realSpec_.samples() / this->realSpec_.byteLength();
	for(Player& player : this->players_){
		SoundSpec const& spec(player.instrument->spec());
		player.buffer.resize(spec.byteLength() * num);
	}
}

void Quartet::notifySoundSpec(SoundSpec const& spec)
{
	if(this->realSpec_ != spec){
		this->realSpec_ = spec;
		this->updateBufferSize();
	}
}

bool Quartet::start()
{
	return this->startImpl();
}
bool Quartet::stop()
{
	return this->stopImpl();
}

bool Quartet::lock() noexcept
{
	return this->lockImpl();
}
bool Quartet::unlock() noexcept
{
	return this->unlockImpl();
}
struct PlayerFinder : public std::unary_function<Quartet::Player, bool> {
private:
	Handler<Instrument> const& player_;
public:
	PlayerFinder(Handler<Instrument> const& p):player_(p){};
	bool operator()(Quartet::Player const& b) {
		return player_ == b.instrument;
	}
};

bool Quartet::addInstrument(Handler<Instrument> const& inst)
{
	if(this->hasInstrument(inst)){
		return false;
	}else{
		inst->onConnected(self(), this->realSpec_);
		this->players_.push_back(Player(inst, inst->spec().byteLength()*(this->realSpec_.samples()/this->realSpec_.byteLength())));
		return true;
	}
}

bool Quartet::hasInstrument(Handler<Instrument> const& inst)
{
	std::vector<Player>::iterator it = std::find_if(this->players_.begin(), this->players_.end(), PlayerFinder(inst));
	return it != this->players_.end();
}
bool Quartet::removeInstrument(Handler<Instrument> const& inst)
{
	std::vector<Player>::iterator it = std::find_if(this->players_.begin(), this->players_.end(), PlayerFinder(inst));
	if(it != this->players_.end()){
		Player& p = *it;
		p.instrument->onDisconnected();
		this->players_.erase(it);
		return true;
	}else{ /* 持ってないですよ */
		return false;
	}
}

void Quartet::onPlay(unsigned char* stream, int len)
{
	if(this->players_.empty()){
		std::memset(stream, 0, len);
		return;
	}
	for(Player& player : this->players_){
		player.instrument->play(player.buffer.data(), player.buffer.size());
	}
	this->playImpl(stream, len);
}

}
