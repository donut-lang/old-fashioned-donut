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

#include "NesGeistObject.h"
#include "../../chisa/tk/World.h"
#include "../machine/debugger/Assembler.h"

namespace nes {
using namespace tarte;

NesGeistProvider::NesGeistProvider(Handler< ::donut::Heap> const& heap)
:Super(heap)
{
	this->registerPureNativeClosure("disasmAt", [](NesGeistObject* self, int addr){
		Instruction inst;
		self->geist()->machine()->debugger().disassembler().decodeAt(addr, inst);
		return inst.toString();
	});
	this->registerReactiveNativeClosure("loadNES",[this](NesGeistObject* obj, std::string fname){
		Handler<NesGeist> geist(obj->geist());
		NesGeist::MachineLock lock(geist);
		NesGeistSideEffect side;
		geist->machine()->loadCartridge((geist->world()->resolveUniverseFilepath(fname)).c_str());
		geist->machine()->sendHardReset();
		geist->machine()->debugger().watcher().stepRunning();

		return std::make_tuple(fname,false,side);
	});
	this->registerReactiveNativeClosure("writeNES", &NesGeistObject::writeMemNES);
	this->registerReactiveNativeClosure("selectReset",[](NesGeistObject* obj){
		Handler<NesGeist> geist(obj->geist());
		NesGeist::MachineLock lock(geist);
		MemoryComparator& cmp = geist->machine()->debugger().comparator();
		NesGeistSideEffect side;
		side.op_before = side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
		side.before = geist->machine()->save();
		cmp.reset();
		side.after = geist->machine()->save();
		return std::make_tuple(cmp.candidates(),true,side);
	});
	this->registerReactiveNativeClosure("selectTakeSnapshot",[](NesGeistObject* obj){
		Handler<NesGeist> geist(obj->geist());
		NesGeist::MachineLock lock(geist);
		MemoryComparator& cmp = geist->machine()->debugger().comparator();
		NesGeistSideEffect side;
		side.op_before = side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
		side.before = geist->machine()->save();
		cmp.takeSnapshot();
		side.after = geist->machine()->save();
		return std::make_tuple(cmp.candidates(),true,side);
	});
#define SELECT_FU(_op) \
	this->registerReactiveNativeClosure("select" #_op,[](NesGeistObject* obj){\
		Handler<NesGeist> geist(obj->geist());\
		NesGeist::MachineLock lock(geist);\
		MemoryComparator& cmp = geist->machine()->debugger().comparator();\
		NesGeistSideEffect side;\
		side.op_before = side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;\
		side.before = geist->machine()->save();\
		cmp.select##_op();\
		side.after = geist->machine()->save();\
		return std::make_tuple(cmp.candidates(),true,side);\
	});
	SELECT_FU(Gt);
	SELECT_FU(Ge);
	SELECT_FU(Lt);
	SELECT_FU(Le);
	SELECT_FU(Eq);
	SELECT_FU(Ne);
#undef SELECT_FU

#define SELECT_FU(_op) \
	this->registerReactiveNativeClosure("selectConst" #_op,[](NesGeistObject* obj, uint8_t val){\
		Handler<NesGeist> geist(obj->geist());\
		NesGeist::MachineLock lock(geist);\
		MemoryComparator& cmp = geist->machine()->debugger().comparator();\
		NesGeistSideEffect side;\
		side.op_before = side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;\
		side.before = geist->machine()->save();\
		cmp.select##_op(val);\
		side.after = geist->machine()->save();\
		return std::make_tuple(cmp.candidates(),true,side);\
	});
	SELECT_FU(Gt);
	SELECT_FU(Ge);
	SELECT_FU(Lt);
	SELECT_FU(Le);
	SELECT_FU(Eq);
	SELECT_FU(Ne);

	this->registerReactiveNativeClosure("addExecBreak",&NesGeistObject::addExecBreak);
	this->registerReactiveNativeClosure("removeExecBreak", &NesGeistObject::removeExecBreak);
	this->registerReactiveNativeClosure("stepRunning",&NesGeistObject::stepRunning);
	this->registerReactiveNativeClosure("continueRunning",&NesGeistObject::continueRunning);
	this->registerReactiveNativeClosure("writeAsmNES",&NesGeistObject::writeAsmNES);
}

//---------------------------------------------------------------------------------------

NesGeistObject::NesGeistObject(NesGeistProvider* provider)
:Super(provider)
{
}

void NesGeistObject::onFutureDiscarded(const Handler< ::donut::Heap>& heap)
{
}

void NesGeistObject::onHistoryDiscarded(const Handler< ::donut::Heap>& heap)
{
}

typename NesGeistObject::ResultType NesGeistObject::onBack(const Handler< ::donut::Heap>& heap, const AntiSideEffect& val)
{
	Handler<NesGeist> geist(this->geist());
	AntiSideEffect anti;
	anti.op_before = val.op_before;
	anti.op_after = val.op_after;
	anti.before = val.before;
	anti.after = val.after;
	switch(val.op_before){
	case AntiSideEffect::None:
		return Super::onBack(heap,anti);
	case AntiSideEffect::LoadSave:{
		NesGeist::MachineLock lock(geist);
		geist->machine()->load(val.before);
		break;

	}
	case AntiSideEffect::LoadSaveAndRun: {
		NesGeist::MachineLock lock(geist);
		geist->machine()->load(val.before);
		geist->machine()->debugger().watcher().stepRunning();
		break;
	}
	}
	return std::tuple<bool, AntiSideEffect>(true, anti);
}

typename NesGeistObject::ResultType NesGeistObject::onForward(const Handler< ::donut::Heap>& heap, const AntiSideEffect& val)
{
	Handler<NesGeist> geist(this->geist());
	AntiSideEffect anti;
	anti.op_before = val.op_before;
	anti.op_after = val.op_after;
	anti.before = val.before;
	anti.after = val.after;
	switch(val.op_after){
	case AntiSideEffect::None:
		return Super::onForward(heap,val);
	case AntiSideEffect::LoadSave: {
		NesGeist::MachineLock lock(geist);
		geist->machine()->load(val.after);
		break;
	}
	case AntiSideEffect::LoadSaveAndRun: {
		NesGeist::MachineLock lock(geist);
		geist->machine()->load(val.after);
		geist->machine()->debugger().watcher().stepRunning();
		break;
	}
	}
	return std::tuple<bool, AntiSideEffect>(true, anti);
}

XValue NesGeistObject::saveImpl(const Handler< ::donut::Heap>& heap)
{
}

void NesGeistObject::loadImpl(const Handler< ::donut::Heap>& heap, const XValue& data)
{
}

std::tuple<std::nullptr_t, bool, NesGeistObject::AntiSideEffect> NesGeistObject::stepRunning()
{
	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	Watcher& watcher = geist->machine()->debugger().watcher();
	NesGeistSideEffect side;
	side.op_before = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	side.before = geist->machine()->save();
	watcher.stepRunning();
	side.after = geist->machine()->save();
	side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	return std::tuple<std::nullptr_t, bool, NesGeistObject::AntiSideEffect>(nullptr,true,side);
}

std::tuple<break_id_t, bool, NesGeistObject::AntiSideEffect> NesGeistObject::addExecBreak(uint16_t addr_first, uint16_t addr_end)
{
	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	Watcher& watcher = geist->machine()->debugger().watcher();
	NesGeistSideEffect side;
	side.op_before = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	side.before = geist->machine()->save();
	break_id_t id = watcher.addMemoryExecBreak(addr_first, addr_end);
	side.after = geist->machine()->save();
	side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;

	return std::tuple<break_id_t, bool, NesGeistObject::AntiSideEffect>(id,true,side);
}

std::tuple<bool, bool, NesGeistObject::AntiSideEffect> NesGeistObject::removeExecBreak(break_id_t id)
{

	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	Watcher& watcher = geist->machine()->debugger().watcher();
	NesGeistSideEffect side;
	side.op_before = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	side.before = geist->machine()->save();
		bool result = watcher.removeMemoryExecBreak(id);
	side.after = geist->machine()->save();
	side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;

	return std::tuple<bool, bool, NesGeistObject::AntiSideEffect>(result, true, side);
}

std::tuple<std::nullptr_t, bool, NesGeistObject::AntiSideEffect> NesGeistObject::continueRunning()
{
	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	Watcher& watcher = geist->machine()->debugger().watcher();
	NesGeistSideEffect side;
	side.op_before = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	side.before = geist->machine()->save();
		watcher.resumeRunning();
	side.after = geist->machine()->save();
	side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	return std::tuple<std::nullptr_t, bool, NesGeistObject::AntiSideEffect>(nullptr,true,side);
}

std::tuple<std::string, bool, NesGeistObject::AntiSideEffect> NesGeistObject::writeAsmNES(uint16_t addr, std::string val)
{
	Handler<NesGeist> geist(this->geist());
	NesGeist::MachineLock lock(geist);
	NesGeistSideEffect side;
	std::string msg;
	Instruction orig;
	geist->machine()->debugger().disassembler().decodeAt(addr, orig);
	side.op_before = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	side.before = geist->machine()->save();
		Instruction inst = encodeInst(val);
		if(inst.op_ == Operation::Invalid) {
			msg = "無効な命令です。";
		}else if( orig.binLength_ < inst.binLength_ ){
			msg = "命令が入りきりません";
		}else{
			for(int i=0;i<inst.binLength_;++i){
				geist->machine()->debuggerWrite(addr+i, inst.bin[i]);
			}
			for(int i=inst.binLength_;i<orig.binLength_;++i){
				geist->machine()->debuggerWrite(addr+i, 0xea);
			}
		}
	side.after = geist->machine()->save();
	side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
	return std::tuple<std::string, bool, NesGeistObject::AntiSideEffect>(msg,true,side);
}

std::tuple<std::string, bool, NesGeistObject::AntiSideEffect> NesGeistObject::writeMemNES(uint16_t addr, std::string vals)
{
	bool succeed;
	if(! ::tarte::startsWith(vals, "0x") ){ //XXX: 今日だけでは
		while(vals[0]=='0') {
			vals = vals.substr(1);
		}
	}
	int val = ::tarte::parseAs<int>(vals, &succeed);
	NesGeistSideEffect side;
	if( !succeed ){
		side.op_before = side.op_after = NesGeistSideEffect::None;
		return std::make_tuple("正しい数字ではありません。",true,side);
	}else if( val < 0 ){
		side.op_before = side.op_after = NesGeistSideEffect::None;
		return std::make_tuple("0以上の数字を入れて下さい。",true,side);
	}else if( val > 255 ){
		side.op_before = side.op_after = NesGeistSideEffect::None;
		return std::make_tuple("255以下の数字を入れて下さい。",true,side);
	}else{
		Handler<NesGeist> geist(this->geist());
		NesGeist::MachineLock lock(geist);
		side.op_before = side.op_after = geist->isBreak() ? NesGeistSideEffect::LoadSave : NesGeistSideEffect::LoadSaveAndRun;
		side.before = geist->machine()->save();
		geist->machine()->debuggerWrite(addr, val);
		side.after = geist->machine()->save();
		return std::make_tuple("",true,side);
	}
}

}
