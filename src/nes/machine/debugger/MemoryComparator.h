/* coding: utf-8 */
/**
 * Chisa
 *
 * Copyright 2012-2013, PSI
 */

#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <climits>
#include <cinamo/ClassUtil.h>

namespace nes {

class VirtualMachine;
class MemoryComparator final {
public:
	MemoryComparator(VirtualMachine& vm);
	~MemoryComparator() noexcept = default;
private:
	VirtualMachine& vm_;
	unsigned int entry_[2048/(CHAR_BIT * sizeof(unsigned int))];
	std::vector<uint8_t> last_;
	unsigned int candidates_;
public:
template <typename Archiver>
	void serialize(Archiver& arc){
		arc & entry_;
		arc & last_;
		arc & candidates_;
	}
private:
	std::vector<uint8_t> getNowMemory() const;
private:
	template <typename Functor>
	inline void selectCompare( Functor const& cmp ) {
		std::vector<uint8_t> now( this->getNowMemory() );
		std::size_t const max = now.size();
		for( uint16_t addr = 0; addr < max; ++addr ) {
			const uint16_t idx = addr / (CHAR_BIT*sizeof(unsigned int));
			const uint16_t mask_bit = addr % (CHAR_BIT*sizeof(unsigned int));
			const unsigned int mask = 1 << mask_bit;
			if( (entry_[idx] & mask) && (!cmp(now[addr], last_[addr])) ) {
				entry_[idx] &= ~mask;
				--this->candidates_;
			}
		}
	}
	template <typename Functor>
	inline void selectConst( Functor const& cmp ) {
		std::vector<uint8_t> now( this->getNowMemory() );
		std::size_t const max = now.size();
		for( uint16_t addr = 0; addr < max; ++addr ) {
			const uint16_t idx = addr / (CHAR_BIT*sizeof(unsigned int));
			const uint16_t mask_bit = addr % (CHAR_BIT*sizeof(unsigned int));
			const unsigned int mask = 1 << mask_bit;
			if( (entry_[idx] & mask) && (!cmp(now[addr])) ) {
				entry_[idx] &= ~mask;
				--this->candidates_;
			}
		}
	}
public:
	inline unsigned int candidates() const noexcept { return this->candidates_; };
	inline bool isCandidate( uint16_t const& addr ) const noexcept { return (entry_[addr / (CHAR_BIT*sizeof(unsigned int))] & (1 << (addr % (CHAR_BIT*sizeof(unsigned int))))); };
	inline bool isScinamod() const noexcept { return !(this->last_.empty()); };
	inline uint8_t last(uint16_t const& addr) const noexcept { return this->last_[addr & 2047]; };
	uint8_t now(uint16_t const& addr) const noexcept;
public:
	void start();
	void reset();
	void takeSnapshot() { this->last_ = getNowMemory(); };
public:
	void selectEq(uint8_t val) { selectConst( std::bind2nd(std::equal_to<uint8_t>(), val) ); };
	void selectEq() { selectCompare(std::equal_to<uint8_t>()); };
	void selectNe(uint8_t val){ selectConst( std::bind2nd(std::not_equal_to<uint8_t>(), val) ); };
	void selectNe() { selectCompare(std::not_equal_to<uint8_t>()); };
	void selectGt(uint8_t val){ selectConst( std::bind2nd(std::greater<uint8_t>(), val) ); };
	void selectGt() { selectCompare(std::greater<uint8_t>()); };
	void selectGe(uint8_t val){ selectConst( std::bind2nd(std::greater_equal<uint8_t>(), val) ); };
	void selectGe() { selectCompare(std::greater_equal<uint8_t>()); };
	void selectLt(uint8_t val){ selectConst( std::bind2nd(std::less<uint8_t>(), val) ); };
	void selectLt() { selectCompare(std::less<uint8_t>()); };
	void selectLe(uint8_t val){ selectConst( std::bind2nd(std::less_equal<uint8_t>(), val) ); };
	void selectLe() { selectCompare(std::less_equal<uint8_t>()); };
};

}
