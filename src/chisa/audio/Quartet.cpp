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
#include <cinamo/Exception.h>
#include <cstring>
#include "Quartet.h"
#include "Instrument.h"

namespace chisa {

Quartet::Player::Player(Handler<Instrument> const& inst, std::size_t buflen)
:instrument(inst), buffer(buflen)
{

}

Quartet::Quartet(SoundSpec const& desired)
:desiredSpec_(desired), startRequested_(false), stared_(false)
{
}

/******************************************************************************************************************
 * 外側からのインターフェイス
 ******************************************************************************************************************/

bool Quartet::start()
{
	bool expected = false;
	if(this->startRequested_.compare_exchange_strong(expected,  true)) {
		return this->startInner();
	}
	return false;
}
bool Quartet::stop()
{
	bool expected = true;
	if(this->startRequested_.compare_exchange_strong(expected,  false)) {
		return this->stopInner();
	}else{
		return false;
	}
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
	if(std::find_if(this->players_.begin(), this->players_.end(), PlayerFinder(inst)) != this->players_.end()){
		return false;
	}
	{
		Lock lock(*this);
		inst->onConnected(self(), this->realSpec_);
		this->players_.push_back(Player(inst, inst->spec().byteLength()*this->realSpec_.samples()));
	}
	if(this->startRequested_.load()){
		this->startInner();
	}
	return true;
}

bool Quartet::hasInstrument(Handler<Instrument> const& inst)
{
	return this->players_.end()
			!= std::find_if(this->players_.begin(), this->players_.end(), PlayerFinder(inst));
}
bool Quartet::removeInstrument(Handler<Instrument> const& inst)
{
	std::vector<Player>::iterator const it = std::find_if(this->players_.begin(), this->players_.end(), PlayerFinder(inst));
	if(it == this->players_.end()){
		return false;
	}
	{
		Lock lock(*this);
		Player& p = *it;
		p.instrument->onDisconnected();
		this->players_.erase(it);
	}
	if(this->players_.empty()){
		this->stopInner();
	}
	return true;
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

/******************************************************************************************************************
 * 実装用
 ******************************************************************************************************************/

void Quartet::notifySoundSpec(SoundSpec::DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples)
{
	this->notifySoundSpec(SoundSpec(format, channels, frequency, samples));
}
void Quartet::notifySoundSpec(SoundSpec const& spec)
{
	if(this->realSpec_ != spec){
		this->realSpec_ = spec;
		this->updateBufferSize();
	}
}

/**********************************************************************************************************************
 * 内部実装
 *********************************************************************************************************************/
void Quartet::updateBufferSize()
{
	int const samples = this->realSpec_.samples();
	Lock lock(*this);
	for(Player& player : this->players_){
		SoundSpec const& spec(player.instrument->spec());
		player.buffer.resize(spec.byteLength() * samples);
	}
}

bool Quartet::lock() noexcept
{
	return this->lockImpl();
}
bool Quartet::unlock() noexcept
{
	return this->unlockImpl();
}

bool Quartet::startInner()
{
	if(this->players_.empty()){
		return false;
	}
	bool expected = false;
	if(this->stared_.compare_exchange_strong(expected,  true)){
		return this->startImpl();
	}
	return false;
}
bool Quartet::stopInner()
{
	bool expected = true;
	if(this->stared_.compare_exchange_strong(expected,  false)){
		return this->startImpl();
	}
	return false;
}

}
