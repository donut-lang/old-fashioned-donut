/**
 * Chisa
 * Copyright (C) 2012 psi
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
#include "../../Handler.h"
#include "Closure.h"
#include <vector>
#include <algorithm>
#include "Inst.h"

namespace chisa {
namespace donut {

template <typename T>
class ConstTable {
private:
	std::vector<T> table_;
public:
	Instruction regist( T const& val ) {
		auto it = std::find(table_.begin(), table_.end(), val);
		if(it == table_.end()){
			table_.push_back(val);
			return table_.size()-1;
		}else{
			unsigned int idx = it-table_.begin();
			return idx;
		}
	}
	T get(std::size_t index) const
	{
		if(index >= table_.size()){
			throw logging::Exception(__FILE__, __LINE__, "Unknown const index: %d", index);
		}
		return table_.at(index);
	}
	std::size_t size() const
	{
		return this->table_.size();
	}
};

class Code : public HandlerBody<Code> {
private:
	ConstTable<int> intTable_;
	ConstTable<float> floatTable_;
	ConstTable<std::string> stringTable_;
	ConstTable<Handler<Closure> > closureTable_;
public:
	Code();
	virtual ~Code() noexcept = default;
	bool onFree() noexcept { return false; }
	template <typename T> Instruction constCode(T const& val);
public:
public:
	std::string disasm( Instruction inst );
	inline void disasm( Instruction inst, Instruction& opcode, Instruction& constKind, int& constIndex ) const {
		opcode = inst & Inst::OpcodeMask;
		constKind = inst & Inst::ConstKindMask;
		constIndex = inst & 0xffff;
		if(constIndex >= 0x8000) {
			constIndex -= 0x10000;
		}
	}
public:
	inline bool getBool(const unsigned int& idx) const noexcept{
		return (idx & 0xffff) == 0 ? false : true;
	}
	inline std::size_t numBool() const noexcept{
		return 2;
	}
	inline int getInt(const unsigned int& idx) const noexcept{
		return intTable_.get(idx & 0xffff);
	}
	inline std::size_t numInt() const noexcept{
		return intTable_.size();
	}
	inline float getFloat(const unsigned int& idx){
		return floatTable_.get(idx & 0xffff);
	}
	inline std::size_t numFloat() const noexcept{
		return floatTable_.size();
	}
	inline std::string getString(const unsigned int& idx){
		return stringTable_.get(idx & 0xffff);
	}
	inline std::size_t numString() const noexcept {
		return stringTable_.size();
	}
	inline Handler<Closure> getClosure(const unsigned int& index) {
		return this->closureTable_.get(index & 0xffff);
	}
	inline std::size_t numClosure() const noexcept
	{
		return this->closureTable_.size();
	}
};

}}
