/* coding: utf-8 */
/**
 * Donut
 *
 * Copyright 2012-2013, PSI
 */

#pragma once

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
	OpcodeMask  		= 0xff<<24,
	OpcodeShift 		= 24,
	Nop				= 0<<24,
	Push				= 1<<24,
	PushCopy			= 2<<24,
	ReplaceCopy				= 3<<24,
	Pop				= 4<<24,
	SearchScope		= 5<<24,
	StoreObj			= 6<<24,
	LoadObj			= 7<<24,
	Apply			= 8<<24,
	ConstructArray	=9<<24,
	ConstructObject	=10<<24,
	Branch			=11<<24,
	BranchTrue		=12<<24,
	BranchFalse		=13<<24,
	PushSelf			=14<<24,
	Interrupt		=15<<24,
	Return			=16<<24,
	//const mask
	ConstKindMask  = 0xff<<16,
	ConstKindShift  = 16,
	ConstOperand= 0<<16,
	ConstInt	= 1<<16,
	ConstBool	= 2<<16,
	ConstFloat	= 3<<16,
	ConstClosure= 4<<16,
	ConstString	= 5<<16,
	ConstNull	= 6<<16
};

}
