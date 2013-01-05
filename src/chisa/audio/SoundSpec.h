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
#include <tuple>
#include <climits>
#include <tarte/ClassUtil.h>
namespace chisa {

class SoundSpec {
	STACK_OBJECT(SoundSpec);
public:
	enum DataFormat {
		INVALID=0,
		U8=0x0008,  /**< Unsigned 8-bit samples */
		S8=0x8008,  /**< Signed 8-bit samples */
		U16LSB=0x0010,  /**< Unsigned 16-bit samples */
		S16LSB=0x8010,  /**< Signed 16-bit samples */
		U16MSB=0x1010,  /**< As above, but big-endian byte order */
		S16MSB=0x9010,  /**< As above, but big-endian byte order */
		U16=U16LSB,
		S16=S16LSB,
		S32LSB=0x8020,  /**< 32-bit integer samples */
		S32MSB=0x9020,  /**< As above, but big-endian byte order */
		S32=S32LSB,
		F32LSB=0x8120,  /**< 32-bit floating point samples */
		F32MSB=0x9120,  /**< As above, but big-endian byte order */
		F32=F32LSB,
/**
 *  \name Native audio byte ordering
 */
#if !IS_BIG_ENDIAN
		U16SYS=U16LSB,
		S16SYS=S16LSB,
		S32SYS=S32LSB,
		F32SYS=F32LSB,
#else
		U16SYS=U16MSB,
		S16SYS=S16MSB,
		S32SYS=S32MSB,
		F32SYS=F32MSB,
#endif
	};
private:
	DataFormat   format_;
	unsigned int channels_;
	unsigned int frequency_;
	unsigned int samples_;
public:
	SoundSpec(DataFormat format, unsigned int channels, unsigned int frequency, unsigned int samples)
	:format_(format), channels_(channels), frequency_(frequency), samples_(samples){}
	SoundSpec()	:format_(DataFormat::INVALID), channels_(0), frequency_(0), samples_(0){}
	SoundSpec(SoundSpec const&) = default;
	SoundSpec(SoundSpec&&) = default;
	SoundSpec& operator=(SoundSpec const&) = default;
	SoundSpec& operator=(SoundSpec&&) = default;
	~SoundSpec() noexcept = default;
public:
	inline DataFormat const& format() const noexcept { return this->format_; };
	inline unsigned int const& channels() const noexcept { return this->channels_; };
	inline unsigned int const& frequency() const noexcept { return this->frequency_; };
	inline unsigned int const& samples() const noexcept { return this->samples_; };
	inline DataFormat& format() noexcept { return this->format_; };
	inline unsigned int& channels() noexcept { return this->channels_; };
	inline unsigned int& frequency() noexcept { return this->frequency_; };
	inline unsigned int& samples() noexcept { return this->samples_; };
public:
	inline bool operator==(SoundSpec const& o) const noexcept {
		return
				this->format_ == o.format_ &&
				this->channels_ == o.channels_ &&
				this->frequency_ == o.frequency_ &&
				this->samples_ == o.samples_;
	}
	inline bool operator!=(SoundSpec const& o) const noexcept {
		return
				this->format_ != o.format_ ||
				this->channels_ != o.channels_ ||
				this->frequency_ != o.frequency_ ||
				this->samples_ != o.samples_;
	}
	inline bool operator>(SoundSpec const& o) const noexcept{
		return
				std::tie(this->format_, this->channels_, this->frequency_, this->samples_) >
				std::tie(    o.format_,     o.channels_,     o.frequency_,     o.samples_);
	}
	inline bool operator>=(SoundSpec const& o) const noexcept{
		return
				std::tie(this->format_, this->channels_, this->frequency_, this->samples_) >=
				std::tie(    o.format_,     o.channels_,     o.frequency_,     o.samples_);
	}
	inline bool operator<(SoundSpec const& o) const noexcept{
		return
				std::tie(this->format_, this->channels_, this->frequency_, this->samples_) <
				std::tie(    o.format_,     o.channels_,     o.frequency_,     o.samples_);
	}
	inline bool operator<=(SoundSpec const& o) const noexcept{
		return
				std::tie(this->format_, this->channels_, this->frequency_, this->samples_) <=
				std::tie(    o.format_,     o.channels_,     o.frequency_,     o.samples_);
	}
	inline bool isInvalid() const noexcept{
		return
				this->format_ == DataFormat::INVALID ||
				this->channels_ <= 0 ||
				this->frequency_ <= 0 ||
				this->samples_ <= 0;
	}
	inline bool isCompatible(SoundSpec const& o) const noexcept{
		return format_ == o.format_ && channels_ == o.channels_ && frequency_ == o.frequency_;
	}
	inline void swap(SoundSpec& o){
		using std::swap;
		swap(format_, o.format_);
		swap(channels_, o.channels_);
		swap(frequency_, o.frequency_);
		swap(samples_, o.samples_);
	}
private:
	static constexpr const unsigned int EndianMask = (1<<12);
	static constexpr const unsigned int SignedMask = (1<<15);
	static constexpr const unsigned int FloatMask = (1<<8);
	static constexpr const unsigned int BitsizeMask = (0xff);
public:
	inline bool isBigEndian() const noexcept{
		return this->format_ & EndianMask;
	}
	inline bool isLittleEndian() const noexcept{
		return !(this->format_ & EndianMask);
	}
	inline bool isSigned() const noexcept{
		return (this->format_ & SignedMask);
	}
	inline bool isUnsigned() const noexcept{
		return !(this->format_ & SignedMask);
	}
	inline bool isFloat() const noexcept{
		return (this->format_ & FloatMask);
	}
	inline bool isInt() const noexcept{
		return !(this->format_ & FloatMask);
	}
	inline int bitLength() const noexcept{
		return this->format_ & BitsizeMask;
	}
	inline int byteLength() const noexcept{
		return ( bitLength() ) / CHAR_BIT;
	}
};

inline void swap(SoundSpec& a,SoundSpec& b)
{
	a.swap(b);
}

}
