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
#include "Quartet.h"
#include "Instrument.h"

namespace chisa {

Quartet::Quartet(SoundSpec const& desired)
:desiredSpec_(desired)
{
}

void Quartet::notifySoundSpec(SoundSpec::DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples)
{
	this->realSpec_ = SoundSpec(format, channels, frequency, samples);
}
void Quartet::notifySoundSpec(SoundSpec const& spec)
{
	this->realSpec_ = spec;
}

bool Quartet::start()
{
	return this->startImpl();
}
bool Quartet::stop()
{
	return this->stopImpl();
}
bool Quartet::addInstrument(Handler<Instrument> const& inst)
{
	if(this->addInstrumentImpl(inst)){
		return true;
	}else if(this->hasInstrument(inst)){
		return false;
	}else{
		this->instruments_.push_back(inst);
		return true;
	}
}

bool Quartet::hasInstrument(Handler<Instrument> const& inst)
{
	if(this->hasInstrumentImpl(inst)) {
		return true;
	}else{
		std::vector<Handler<Instrument> >::iterator it = std::find(this->instruments_.begin(), this->instruments_.end(), inst);
		return it != this->instruments_.end();
	}
}
bool Quartet::removeInstrument(Handler<Instrument> const& inst)
{
	if(this->removeInstrumentImpl(inst)){
		return true;
	}else{
		std::vector<Handler<Instrument> >::iterator it = std::find(this->instruments_.begin(), this->instruments_.end(), inst);
		if(it != this->instruments_.end()){
			this->instruments_.erase(it);
			return true;
		}else{ /* 持ってないですよ */
			return false;
		}
	}
}


}
