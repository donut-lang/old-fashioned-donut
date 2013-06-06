/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#include "Instrument.h"
#include "Quartet.h"

namespace chisa {

Instrument::Instrument()
{
}


Handler<Quartet> Instrument::quartet() const
{
	return this->quartet_.lock();
}
SoundSpec Instrument::onConnected(Handler<Quartet> const& quartet, SoundSpec const& spec)
{
	this->quartet_ = quartet;
	return this->spec_ = this->querySpec(spec);
}

void Instrument::onDisconnected()
{
	this->quartet_.reset();
	SoundSpec().swap(this->spec_);
}

SoundSpec Instrument::spec() noexcept
{
	return this->spec_;
}
void Instrument::play(unsigned char *stream, int len)
{
	this->playImpl(stream, len);
}

}
