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
#include <vector>
#include <cstdint>
#include <functional>
#include <climits>
#include <tarte/ClassUtil.h>

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
		last_.swap(now);
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
		last_.swap(now);
	}
public:
	inline unsigned int candidates() const noexcept { return this->candidates_; };
	inline bool isCandidate( uint16_t const& addr ) const noexcept { return (entry_[addr / (CHAR_BIT*sizeof(unsigned int))] & (1 << (addr % (CHAR_BIT*sizeof(unsigned int))))); };
	inline uint8_t last(uint16_t const& addr) const noexcept { return this->last_[addr & 0x2047]; };
	uint8_t now(uint16_t const& addr) const noexcept;
public:
	void start();
	void reset();
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
