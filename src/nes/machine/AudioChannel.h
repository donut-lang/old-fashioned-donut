/*
 * AudioChannel.h
 *
 *  Created on: 2011/08/25
 *      Author: psi
 */

#pragma once
#include <cstddef>
#include <stdint.h>
#include <stdio.h>

namespace nes {
class VirtualMachine;

class AudioChannel
{
public:
	static const uint8_t LengthCounterConst[32];
};

class Rectangle final {
private:
	bool isFirstChannel;

	//decay or volume
	uint8_t volumeOrDecayRate;
	bool decayReloaded;
	bool decayEnabled;
	uint8_t dutyRatio;

	uint8_t decayCounter;

	uint8_t decayVolume;

	//sweep
	bool sweepEnabled;
	uint8_t sweepShiftAmount;
	bool sweepIncreased;
	uint8_t sweepUpdateRatio;

	uint8_t sweepCounter;

	//
	uint16_t frequency;
	bool loopEnabled;
	uint8_t lengthCounter;

	//
	uint16_t freqCounter;
	uint16_t dutyCounter;

public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & isFirstChannel;
		//decay or volume
		arc & volumeOrDecayRate;
		arc & decayReloaded;
		arc & decayEnabled;
		arc & dutyRatio;

		arc & decayCounter;

		arc & decayVolume;

		arc & sweepEnabled;
		arc & sweepShiftAmount;
		arc & sweepIncreased;
		arc & sweepUpdateRatio;

		arc & sweepCounter;
	}

public:
	explicit Rectangle(bool isFirstChannel);
	~Rectangle() noexcept = default;
	inline void analyzeVolumeRegister(uint8_t reg);
	inline void analyzeSweepRegister(uint8_t reg);
	inline void analyzeFrequencyRegister(uint8_t reg);
	inline void analyzeLengthRegister(uint8_t reg);
	inline void onQuaterFrame();
	inline void onHalfFrame();
	inline int16_t createSound(unsigned int deltaClock);
	inline void setEnabled(bool enabled);
	inline bool isEnabled();
	inline void onHardReset();
	inline void onReset();
};

class Triangle final{
private:
	const static uint8_t waveForm[32];

	bool haltFlag;

	bool enableLinearCounter;
	uint16_t frequency;
	uint16_t linearCounterBuffer;
	//
	uint16_t linearCounter;
	uint16_t lengthCounter;
	//
	uint16_t freqCounter;
	uint16_t streamCounter;

public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & haltFlag;

		arc & enableLinearCounter;
		arc & frequency;
		arc & linearCounterBuffer;

		arc & linearCounter;
		arc & lengthCounter;

		arc & freqCounter;
		arc & streamCounter;
	}
public:
	Triangle();
	~Triangle() noexcept = default;
	inline void analyzeLinearCounterRegister(uint8_t value);
	inline void analyzeFrequencyRegister(uint8_t value);
	inline void analyzeLengthCounter(uint8_t value);
	inline int16_t createSound(unsigned int deltaClock);
	inline void onHardReset();
	inline void onReset();
	inline void onQuaterFrame();
	inline void onHalfFrame();
	inline void setEnabled(bool enabled);
	inline bool isEnabled();
};

class Noize final {
private:
	static const uint16_t FrequencyTable[16];

	//rand
	uint16_t shiftRegister;
	bool modeFlag;

	//decay
	uint8_t volumeOrDecayRate;
	bool decayReloaded;
	bool decayEnabled;

	uint8_t decayCounter;
	uint8_t decayVolume;
	//
	bool loopEnabled;
	uint16_t frequency;
	//
	uint16_t lengthCounter;
	//
	uint16_t freqCounter;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		//rand
		arc & shiftRegister;
		arc & modeFlag;

		//decay
		arc & volumeOrDecayRate;
		arc & decayReloaded;
		arc & decayEnabled;

		arc & decayCounter;
		arc & decayVolume;
		//
		arc & loopEnabled;
		arc & frequency;
		//
		arc & lengthCounter;
		//
		arc & freqCounter;
	}
public:
	Noize();
	~Noize() noexcept = default;
	inline void analyzeVolumeRegister(uint8_t reg);
	inline void analyzeFrequencyRegister(uint8_t reg);
	inline void analyzeLengthRegister(uint8_t reg);
	inline void onQuaterFrame();
	inline void onHalfFrame();
	inline int16_t createSound(unsigned int deltaClock);
	inline void setEnabled(bool enabled);
	inline bool isEnabled();
	inline void onHardReset();
	inline void onReset();
};

class Digital final{
private:
	VirtualMachine& vm_;

	//
	bool irqEnabled;
	bool loopEnabled;
	uint16_t frequency;
	uint8_t deltaCounter;
	uint16_t sampleAddr;
	uint16_t sampleLength;
	uint16_t sampleLengthBuffer;
	const static uint16_t FrequencyTable[16];
	//
	uint8_t sampleBuffer;
	uint8_t sampleBufferLeft;
	//
	uint16_t freqCounter;
public:
	template <typename Archiver>
	void serialize(Archiver& arc){
		arc & irqEnabled;
		arc & loopEnabled;
		arc & frequency;
		arc & deltaCounter;
		arc & sampleAddr;
		arc & sampleLength;
		arc & sampleLengthBuffer;
		//
		arc & sampleBuffer;
		arc & sampleBufferLeft;
		//
		arc & freqCounter;
	}
public:
	Digital(VirtualMachine& vm);
	~Digital() noexcept = default;
	inline void analyzeFrequencyRegister(uint8_t value);
	inline void analyzeDeltaCounterRegister(uint8_t value);
	inline void analyzeSampleAddrRegister(uint8_t value);
	inline void analyzeSampleLengthRegister(uint8_t value);
	inline void next();
	inline int16_t createSound(unsigned int deltaClock);
	inline void setEnabled(bool enabled);
	inline bool isEnabled();
	inline bool isIRQEnabled();
	inline bool isIRQActive();
	inline void onHardReset();
	inline void onReset();
};

}
