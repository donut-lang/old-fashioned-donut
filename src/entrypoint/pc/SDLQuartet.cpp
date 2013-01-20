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

#include <cstring>
#include "SDLQuartet.h"

namespace chisa {

void SDLQuartet::__callback(void* userdata, Uint8* stream, int len)
{
	static_cast<SDLQuartet*>(userdata)->onPlay(stream, len);
}

SDLQuartet::SDLQuartet(const SoundSpec& desired)
:Quartet(desired)
{
	SDL_AudioSpec spec;
	spec.channels = desired.channels();
	spec.format = desired.format();
	spec.freq = desired.frequency();
	spec.samples = desired.samples();
	spec.callback = __callback;
	spec.userdata = this;
	SDL_AudioSpec obtained;
	if (SDL_OpenAudio(&spec, &obtained) != 0) {
		TARTE_EXCEPTION(Exception, "[BUG] Failed to open audio. SDL says: \"%s\"", SDL_GetError());
	}
	this->notifySoundSpec(SoundSpec::DataFormat(obtained.format), obtained.channels, obtained.freq, obtained.samples);
}

SDLQuartet::~SDLQuartet() noexcept
{
	Lock lock(*this);
	SDL_PauseAudio(1);
	SDL_CloseAudio();
}

bool SDLQuartet::lockImpl() noexcept
{
	SDL_LockAudio();
	return true;
}

bool SDLQuartet::unlockImpl() noexcept
{
	SDL_UnlockAudio();
	return true;
}

void SDLQuartet::playImpl(unsigned char* stream, const int len)
{
	if (this->players().size() == 1) { //最適化：ミックスしなくていい
		Player& first = this->players().front();
		SoundSpec const& firstSpec = first.instrument->spec();
		if (this->spec().isCompatibleTo(firstSpec)) { //最適化：変換もしなくていい
			std::copy(first.buffer.begin(), first.buffer.end(), stream);
		} else { //変換必要
			SDL_AudioCVT cvt;
			int const result = SDL_BuildAudioCVT(&cvt, SDL_AudioFormat(firstSpec.format()), firstSpec.channels(), firstSpec.frequency(), SDL_AudioFormat(this->spec().format()), this->spec().channels(), this->spec().frequency());
			if (result == 0) {
				TARTE_EXCEPTION(Exception, "[BUG] Oops. It is not need to convert.")
			} else if (result < 0) {
				TARTE_EXCEPTION(Exception, "[BUG] Oops. This format is not compatible. SDL says: \"%s\"", SDL_GetError())
			}
			const unsigned int temporaryBufferSize = first.buffer.size() * cvt.len_mult;
			bool needCopy = true;
			if (temporaryBufferSize <= static_cast<unsigned int>(len)) {
				std::copy(first.buffer.begin(), first.buffer.end(), stream);
				cvt.buf = stream;
				cvt.len = first.buffer.size();
				needCopy = false;
			} else if (temporaryBufferSize <= first.buffer.size()) {
				cvt.buf = first.buffer.data();
				cvt.len = first.buffer.size();
			} else {
				this->mixBuffer_.resize(temporaryBufferSize);
				std::copy(first.buffer.begin(), first.buffer.end(), this->mixBuffer_.begin());
				cvt.buf = reinterpret_cast<unsigned char*>(this->mixBuffer_.data());
				cvt.len = first.buffer.size();
			}
			if (SDL_ConvertAudio(&cvt) != 0) {
				TARTE_EXCEPTION(Exception, "[BUG] Oops. Failed to convert audio. SDL says: \"%s\"", SDL_GetError())
			} else if (cvt.len_cvt != len) {
				TARTE_EXCEPTION(Exception, "[BUG] Oops. Buffer size does not match! converted: %d != stream: %d", cvt.len_cvt, stream);
			}
			if (needCopy) {
				std::memcpy(stream, cvt.buf, cvt.len_cvt);
			}
		}
		return;
	}
	/**************************************************
	 * 複数チャンネル
	 **************************************************/
	std::memset(stream, 0, len);
	const unsigned int volume = 128 / this->players().size();
	for (Player& player : this->players()) {
		SoundSpec const& spec(player.instrument->spec());
		if (spec.isCompatibleTo(this->spec())) { //変換必要なし
			SDL_MixAudio(stream, player.buffer.data(), len, volume);
			continue;
		}
		SDL_AudioCVT cvt;
		int const result = SDL_BuildAudioCVT(&cvt, SDL_AudioFormat(spec.format()), spec.channels(), spec.frequency(), SDL_AudioFormat(this->spec().format()), this->spec().channels(), this->spec().frequency());
		if (result == 0) {
			TARTE_EXCEPTION(Exception, "[BUG] Oops. It is not need to convert.")
		} else if (result < 0) {
			TARTE_EXCEPTION(Exception, "[BUG] Oops. This format is not compatible. SDL says: \"%s\"", SDL_GetError())
		}
		const unsigned int temporaryBufferSize = player.buffer.size() * cvt.len_mult;
		if (temporaryBufferSize <= player.buffer.size()) {
			cvt.buf = player.buffer.data();
			cvt.len = player.buffer.size();
		} else {
			this->mixBuffer_.resize(temporaryBufferSize);
			std::copy(player.buffer.begin(), player.buffer.end(), this->mixBuffer_.begin());
			cvt.buf = reinterpret_cast<unsigned char*>(this->mixBuffer_.data());
			cvt.len = player.buffer.size();
		}
		if (SDL_ConvertAudio(&cvt) != 0) {
			TARTE_EXCEPTION(Exception, "[BUG] Oops. Failed to convert audio. SDL says: \"%s\"", SDL_GetError())
		} else if (cvt.len_cvt != len) {
			TARTE_EXCEPTION(Exception, "[BUG] Oops. Buffer size does not match! converted: %d != stream: %d", cvt.len_cvt, stream);
		}
		SDL_MixAudio(stream, cvt.buf, cvt.len_cvt, volume);
	}
}

bool SDLQuartet::stopImpl()
{
	SDL_PauseAudio(1);
	return true;
}

bool SDLQuartet::startImpl()
{
	SDL_PauseAudio(0);
	return true;
}

}
