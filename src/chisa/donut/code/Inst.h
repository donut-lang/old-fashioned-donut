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
#include <string>

namespace chisa {
namespace donut {

/** 命令規約
 *　・push/popを除き、命令は必要な分のスタックを消費し、＊必ず値を一つ残す＊
 *　・スタックの載せ方は、popする順に
 *　１：操作対象オブジェクト
 *　（２：メソッドオブジェクト）
 *　（３：引数）
 *　とすること
 */

typedef unsigned int Instruction;
enum Inst {
	// inst
	OpcodeMask  = 0xff<<24,
	OpcodeShift = 24,
	Nop				= 0<<24,
	Push			= 1<<24,
	PushCopy		= 2<<24,
	Pop				= 3<<24,
	SearchScope		= 4<<24,
	StoreObj		= 5<<24,
	LoadObj			= 6<<24,
	LoadLocal		= 7<<24,
	StoreLocal		= 8<<24,
	Apply			= 9<<24,
	ConstructArray	=10<<24,
	ConstructObject	=11<<24,
	//const mask
	ConstKindMask  = 0xff<<16,
	ConstKindShift  = 16,
	ConstOperand= 0<<16,
	ConstInt	= 1<<16,
	ConstBool	= 2<<16,
	ConstFloat	= 3<<16,
	ConstClosure= 4<<16,
	ConstString	= 5<<16
};

}}
