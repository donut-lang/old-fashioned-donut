/*
 * AudioFairy.h
 *
 *  Created on: 2011/08/28
 *      Author: psi
 */

#pragma once

#include <cstring>
#include <tarte/XArchiver.h>
#include "../exception/EmulatorException.h"
#include <tarte/Thread.h>

namespace nes {

class AudioFairy
{
private:
	enum{
		INTERNAL_BUFFER_SIZE = 0x40000,
	};
	int16_t soundBuffer[INTERNAL_BUFFER_SIZE];
	std::atomic<int> lastIndex;
	std::atomic<int> firstIndex;
public:
	AudioFairy():
		lastIndex(0),
		firstIndex(0)
	{
	}
	virtual ~AudioFairy() noexcept = default;
	inline bool pushAudio(int16_t sound)
	{
		const int nowFirstIndex = firstIndex;
		const int available = nowFirstIndex > lastIndex ? nowFirstIndex - lastIndex - 1 : INTERNAL_BUFFER_SIZE-(lastIndex-nowFirstIndex) - 1;
		if(available > 0){
			soundBuffer[lastIndex] = sound;
			lastIndex = (lastIndex+1) & (INTERNAL_BUFFER_SIZE-1);
			return true;
		}else{
			return false;
		}
	}
public:
	void save(::tarte::XArchiverOut& out){
		//out & soundBuffer & lastIndex & firstIndex;
		this->saveImpl(out);
	}
	void load(::tarte::XArchiverIn& in){
		//in & soundBuffer & lastIndex & firstIndex;
		this->loadImpl(in);
	}
protected:
	inline int popAudio(int16_t* buff, int maxLength)
	{
		const int nowLastIndex = lastIndex;
		const int nowFirstIndex = firstIndex;
		const int available = nowFirstIndex <= nowLastIndex ? nowLastIndex-nowFirstIndex : INTERNAL_BUFFER_SIZE-(nowFirstIndex-nowLastIndex);
		const int copiedLength = std::min(available, maxLength);
		if(nowFirstIndex + copiedLength < INTERNAL_BUFFER_SIZE){
			std::memcpy(buff, &soundBuffer[nowFirstIndex], sizeof(int16_t) * copiedLength);
			firstIndex += copiedLength;
		}else{
			const int first = INTERNAL_BUFFER_SIZE-nowFirstIndex;
			const int last = copiedLength-first;
			std::memcpy(buff, &soundBuffer[nowFirstIndex], sizeof(int16_t) * first);
			std::memcpy(&buff[first], &soundBuffer[0], sizeof(int16_t) * last);
			firstIndex = last;
		}
		return copiedLength;
	}
	virtual void saveImpl(::tarte::XArchiverOut& arc) {};
	virtual void loadImpl(::tarte::XArchiverIn& arc) {};
};

class DummyAudioFairy : public AudioFairy
{

};

}
