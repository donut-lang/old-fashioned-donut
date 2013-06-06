/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
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
